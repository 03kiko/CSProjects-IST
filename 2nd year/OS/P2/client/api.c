#include "api.h"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "../common/constants.h"
#include "../common/io.h"
#include "../common/op_codes.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static int client_session_id = -1; 
static int req_pipe = -1;
static int resp_pipe = -1;
static const char* req_path = NULL;
static const char* resp_path = NULL;

int close_client_pipes() {
	if (!req_path || !resp_path || req_pipe < 0 || resp_pipe < 0)
		return 1;
	int failed = close(req_pipe) != 0;
	failed |= close(resp_pipe) != 0;
	failed |= unlink(req_path) != 0;
	failed |= unlink(resp_path) != 0;
	req_pipe = -1;
	resp_pipe = -1;
	req_path = NULL;
	resp_path = NULL;
	return failed;
}

/// Copies src to dest with padding
/// @param dest Destination string to store the result
/// @param src Source string to be copied
/// @param src_len Length of the source strig
/// @param dest_len Length of the destination string
/// @return 0 if successfull; 0 otherwise
/// @note The dest string should have enough space to accomodate src and
///		  additional padding up to dest_len
static int get_padded_str(char* dest, const char* src, size_t src_len,
						   size_t dest_len) {
	if(src_len > dest_len)
		return 1;
	strncpy(dest, src, src_len);
	for(size_t i = src_len; i < dest_len; i++)
		dest[i] = '\0';
	return 0;
}

int ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path) {
	size_t req_len = strlen(req_pipe_path); // concatenate the full request
	size_t resp_len = strlen(resp_pipe_path);
	char padded_req_pipe_path[MAX_CLIENT_PIPE_SIZE];
	char padded_resp_pipe_path[MAX_CLIENT_PIPE_SIZE];
	if (get_padded_str(padded_req_pipe_path, req_pipe_path, req_len,
				   	  MAX_CLIENT_PIPE_SIZE)) {
		print_str(STDERR_FILENO, "Request path is bigger than allowed\n");
		return 1;
	}
	if (get_padded_str(padded_resp_pipe_path, resp_pipe_path, resp_len,
				   	   MAX_CLIENT_PIPE_SIZE)) {
		print_str(STDERR_FILENO, "Request path is bigger than allowed\n");
		return 1;
	}

	size_t msg_size = sizeof(char) + 2 * MAX_CLIENT_PIPE_SIZE;
	char msg[msg_size];

	msg[0] = (char) OP_SETUP;
	memcpy(msg + 1, padded_req_pipe_path, MAX_CLIENT_PIPE_SIZE);
	memcpy(msg + 1 + MAX_CLIENT_PIPE_SIZE, padded_resp_pipe_path, MAX_CLIENT_PIPE_SIZE);

	if (mkfifo(req_pipe_path, 0666) != 0) { // rw-rw-rw
		if (errno == EEXIST)
			print_str(STDERR_FILENO, "Error initializing client: request pipe already in use\n");
		else
			print_str(STDERR_FILENO, "Error initializing client: failed to create request fifo\n");
		return 1;
	}

	if (mkfifo(resp_pipe_path, 0666) != 0) { // rw-rw-rw
		if (errno == EEXIST)
			print_str(STDERR_FILENO, "Error initializing client: response pipe already in use\n");
		else
			print_str(STDERR_FILENO, "Error initializing client: failed to create response fifo\n");
		unlink(req_pipe_path);
		return 1;
	}

	int server_pipe = open(server_pipe_path, O_WRONLY);
	if (server_pipe == -1) {
		print_str(STDERR_FILENO, "Error initializing client: failed to open register fifo (write)\n");
		unlink(req_pipe_path);
		unlink(resp_pipe_path);
		return 1;
	}

	if(write_str(server_pipe, msg, msg_size)) {
		print_str(STDERR_FILENO, "Error initializing client: failed to send registration message\n");
		close(server_pipe);
		return 1;
	}
	if (close(server_pipe) != 0) {
		print_str(STDERR_FILENO, "Error initializing client: failed to close register fifo\n");
		unlink(req_pipe_path);
		unlink(resp_pipe_path);
		return 1;
	}
	req_pipe = open(req_pipe_path, O_WRONLY);
	if(req_pipe == -1) {
		print_str(STDERR_FILENO, "Error initializing client: failed to open request fifo\n");
		unlink(resp_pipe_path);
		return 1;
	}
	resp_pipe = open(resp_pipe_path, O_RDONLY);
	if(resp_pipe == -1) {
		print_str(STDERR_FILENO, "Error initializing client: failed to open response fifo\n");
		close(req_pipe);
		req_pipe = -1;
		unlink(req_pipe_path);
		unlink(resp_pipe_path);
		return 1;
	}
	int session_id = -1;
	if(read_int(resp_pipe, &session_id) != 0) {
		print_str(STDERR_FILENO, "Error initializing client: failed to read session id\n");
		close_client_pipes();
		return 1;
	}
	if (session_id < 0) {
		print_str(STDERR_FILENO, "Error initializing client: invalid session id\n");
		close_client_pipes();
		return 1;
	}
	client_session_id = session_id;
	req_path = req_pipe_path;
	resp_path = resp_pipe_path;
	return 0;
}

int ems_quit(void) {
	if (client_session_id == -1)
		return 1;
	int failed = write_char(req_pipe, (char) OP_QUIT) != 0;
	failed |= write_int(req_pipe, client_session_id) != 0;
	failed |= close_client_pipes();
	client_session_id = -1;
	return failed;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {
	if (write_char(req_pipe, (char) OP_CREATE) || // check if something
		write_int(req_pipe, client_session_id) || // failed to write
		write_uint(req_pipe, event_id) || // if so it must be because
		write_size_t(req_pipe, num_rows) || // segpipe was caught
		write_size_t(req_pipe, num_cols)) { // which means the server died
		print_str(STDERR_FILENO, "ems_create: server disconnected (caught SIGPIPE)\n");
		return -1;
	}

	int return_value;
	if (read_int(resp_pipe, &return_value) != 0) {
		print_str(STDERR_FILENO, "ems_create: server disconnected (found EOF)\n");
		return -1; // this fails if the server closed their end
	}
	return return_value;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {
	if (write_char(req_pipe, (char) OP_RESERVE) ||
		write_int(req_pipe, client_session_id) ||
		write_uint(req_pipe, event_id) ||
		write_size_t(req_pipe, num_seats) ||
		write_size_t_array(req_pipe, xs, num_seats) ||
		write_size_t_array(req_pipe, ys, num_seats)) {
		print_str(STDERR_FILENO, "ems_reserve: server disconnected (caught SIGPIPE)\n");
		return -1;
	}

	int return_value = 1;
	if (read_int(resp_pipe, &return_value) != 0) {
		print_str(STDERR_FILENO, "ems_reserve: server disconnected (found EOF)\n");
		return -1;
	}
	return return_value;
}

int ems_show(int out_fd, unsigned int event_id) {
	if (write_char(req_pipe, (char) OP_SHOW) ||
		write_int(req_pipe, client_session_id) ||
		write_uint(req_pipe, event_id)) {
		print_str(STDERR_FILENO, "ems_show: server disconnected (caught SIGPIPE)\n");
		return -1;
	}

	int return_value = 1;
	size_t num_rows = 0;
	size_t num_cols = 0;
	if (read_int(resp_pipe, &return_value) != 0) {
		print_str(STDERR_FILENO, "ems_show: server disconnected (found EOF)\n");
		return -1;
	}
	if (return_value != 0)
		return 1;
	if (read_size_t(resp_pipe, &num_rows) ||
		read_size_t(resp_pipe, &num_cols)) {
		print_str(STDERR_FILENO, "ems_show: server disconnected (found EOF)\n");
		return -1;
	}

	int failed = 0;
	for (size_t i = 0; i < num_rows; i++) {
		for (size_t j = 0; j < num_cols; j++) {
			unsigned int seat;
			if (read_uint(resp_pipe, &seat) != 0) {
				print_str(STDERR_FILENO, "ems_show: server disconnected (found EOF)\n");
				return -1;
			}
			failed |= print_uint(out_fd, seat) != 0;
			if (j < num_cols - 1)
				failed |= print_str(out_fd, " ");
			else
				failed |= add_line(out_fd);
		}
	}
	if (failed)
		print_str(STDERR_FILENO, "Failed to write seats\n");
	return failed;
}

int ems_list_events(int out_fd) {
	if ((write_char(req_pipe, (char) OP_LIST) != 0) ||
		(write_int(req_pipe, client_session_id) != 0)) {
		print_str(STDERR_FILENO, "ems_list_events: server disconnected (SIGPIPE)\n");
		return -1;
	}

	int return_value = 1;
	size_t num_events = 0;
	if (read_int(resp_pipe, &return_value) != 0) {
		print_str(STDERR_FILENO, "ems_list_events: server disconnected (found EOF)\n");
		return -1;
	}
	if (return_value != 0)
		return 1;
	if (read_size_t(resp_pipe, &num_events) != 0) {
		print_str(STDERR_FILENO, "ems_list_events: server disconnected (found EOF)\n");
		return -1;
	}
	int failed = 0;
	if(num_events == 0)
		print_str(STDERR_FILENO, "No events\n");
	for (size_t i = 0; i < num_events; i++) {
		unsigned int id;
		if (read_uint(resp_pipe, &id) != 0) {
			print_str(STDERR_FILENO, "ems_list_events: server disconnected (found EOF)\n");
			return -1;
		}
		failed |= print_str(out_fd, "Event: ") != 0;
		failed |= print_uint(out_fd, id) != 0;
		failed |= add_line(out_fd);
	}
	if (failed)
		print_str(STDERR_FILENO, "Failed to write event\n");
	return failed;
}
