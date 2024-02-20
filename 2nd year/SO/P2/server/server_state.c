#include "server_state.h"
#include "session.h"
#include "../common/io.h"
#include "../common/op_codes.h"
#include "server_io.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include "operations.h"

#define UNINITIALIZED_SERVER_STATE {.sigusr1_flag = 0, .reg_fd_read = -1, .reg_pipe_path = NULL, .queue = NULL}

typedef struct Server { // holds the state of the server
	int8_t sigusr1_flag; // signals to the host if it received SIGUSR1
	int reg_fd_read; // register pipe file descriptor for reading
	const char* reg_pipe_path; // name of the pipe (not dinamically allocated)
	pthread_t workers[MAX_SESSION_COUNT]; // tid of workers
	SessionQueue* queue; // producer-consumer queue of sessions
} Server;

static Server server = UNINITIALIZED_SERVER_STATE;

/// Executes a create request read from the file described by req_fd
/// and answers into the file described by res_fd. Assumes the
/// op code and session id has already been read.
/// @param req_fd request pipe file descriptor
/// @param res_fd response pipe file descriptor
/// @return 0 in case of success; IO_FAILURE in case
/// the server could not read/write the message successfully
/// (because the client disconnected for instance).
static int execute_create_request(int req_fd, int res_fd) {
	unsigned int event_id;
	size_t num_rows;
	size_t num_cols;
	if (parse_create_request(req_fd, &event_id, &num_rows, &num_cols) != 0) {
		fprintf(stderr, "Failed to parse create request\n");
		return IO_FAILURE;
	}
	if (write_int(res_fd, ems_create(event_id, num_rows, num_cols)) != 0) {
		fprintf(stderr, "Failed to write create response\n");
		return IO_FAILURE;
	}
	return 0;
}

/// Executes a reserve request read from the file described by req_fd
/// and answers into the file described by res_fd. Assumes the
/// op code and session id has already been read.
/// @param req_fd request pipe file descriptor
/// @param res_fd response pipe file descriptor
/// @return 0 in case of success; IO_FAILURE in case
/// the server could not read/write the message successfully
/// (because the client disconnected for instance).
static int execute_reserve_request(int req_fd, int res_fd) {
	unsigned int event_id;
	size_t num_seats;
	size_t xs[MAX_RESERVATION_SIZE];
	size_t ys[MAX_RESERVATION_SIZE];
	if (parse_reserve_request(req_fd, &event_id, &num_seats, xs, ys) != 0) {
		fprintf(stderr, "Failed to parse reserve request\n");
		return IO_FAILURE;
	}
	if (write_int(res_fd, ems_reserve(event_id, num_seats, xs, ys)) != 0) {
		fprintf(stderr, "Failed to write reserve response\n");
		return IO_FAILURE;
	}
	return 0;
}

/// Executes a show request read from the file described by req_fd
/// and answers into the file described by res_fd. Assumes the
/// op code and session id has already been read.
/// @param req_fd request pipe file descriptor
/// @param res_fd response pipe file descriptor
/// @return 0 in case of success; IO_FAILURE in case
/// the server could not read/write the message successfully
/// (because the client disconnected for instance).
static int execute_show_request(int req_fd, int res_fd) {
	unsigned int event_id;
	if (parse_show_request(req_fd, &event_id) != 0) {
		fprintf(stderr, "Failed to parse show request\n");
		return IO_FAILURE;
	}
	if (ems_show(res_fd, event_id) == IO_FAILURE) {
		fprintf(stderr, "Failed to write show response\n");
		return IO_FAILURE;
	}
	return 0;
}

/// Executes a list request and returns the result into
/// the file described by res_fd.
/// @param res_fd response pipe file descriptor
/// @return 0 in case of success; IO_FAILURE in case
/// the server could not read/write the message successfully
/// (because the client disconnected for instance).
static int execute_list_request(int res_fd) {
	if (ems_list_events(res_fd) == IO_FAILURE) {
		fprintf(stderr, "Failed to write list response\n");
		return IO_FAILURE;
	}
	return 0;
}

/// Executes a request from a client.
/// @param req_fd request file descriptor.
/// @param res_fd response file descriptor.
/// @param worker_id worker thread id.
static void execute_requests(int req_fd, int res_fd, int worker_id) {
	while (1) {
		OPCode op_code;
		int client_id = -1;
		int client_io_failure = 0;
		if (get_op_code(req_fd, &op_code) != 0) {
			fprintf(stderr, "Failed to read OP code\n");
			write_int(res_fd, 1);
			break;
		}
		if (read_int(req_fd, &client_id) != 0 || client_id != worker_id) {
			fprintf(stderr, "Illegal id (failed to read or different from expected id)\n");
			if (op_code != OP_QUIT)
				write_int(res_fd, 1);
			break;
		}
		switch (op_code) {
		case OP_CREATE:
			client_io_failure = execute_create_request(req_fd, res_fd);
			break;
		case OP_RESERVE:
			client_io_failure = execute_reserve_request(req_fd, res_fd);
			break;
		case OP_SHOW:
			client_io_failure = execute_show_request(req_fd, res_fd);
			break;
		case OP_LIST:
			client_io_failure = execute_list_request(res_fd);
			break;
		case OP_QUIT:
			return;
		case OP_SETUP:
		default:
			fprintf(stderr, "Invalid OP code\n");
			write_int(res_fd, 1);
			client_io_failure = IO_FAILURE;
		}
		if (client_io_failure != 0) {
			fprintf(stderr, "IO with client failed, closing session\n");
			break;
		}
	}
}

/// Worker thread loop that follows the following pattern:
/// 1. Fetch session from producer-consumer queue.
/// 2. Send message to the client acknowleding connection.
/// 3. Execute client requests.
/// 4. Close session.
/// 5. Repeat.
/// @param worker_id pointer to an integer identifying the worker.
/// @return the void* returned is always NULL.
static void* worker_loop(void* worker_id) {
	sigset_t mask;
	if (!worker_id) {
		fprintf(stderr, "Thread function arguments do not provide id\n");
		return NULL;
	}
	int id = *((int*) worker_id);
	free(worker_id);
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
		fprintf(stderr, "Failed to block SIGUSR1 signal\n");
		return NULL;
	}

	while (1) {
		Session* session = session_queue_fetch(server.queue);
		if (!session) // this means the server stopped running
			break;
		if (session_open(session) != 0) {
			fprintf(stderr, "Failed to open session pipes\n");
			continue;
		}
		if (write_int(session_response_fd(session), id) != 0) {
			fprintf(stderr, "Failed to acknowledge client\n");
			session_free(session);
			continue;
		}
		execute_requests(session_request_fd(session),
				   session_response_fd(session), id);
		if (session_free(session) != 0)
			fprintf(stderr, "Failed to fully free session\n");
	}
	return NULL;
}

/// Checks if the server has already been initialized.
/// @return 1 if initialized; 0 otherwise.
static int server_initialized() {
	int initialized = server.reg_pipe_path != NULL;
	initialized |= server.reg_fd_read > 0;
	initialized |= server.queue != NULL;
	return initialized;
}

/// Checks if a sigusr1 signal occured.
/// @return 1 if it occured, in which case the flag will also
/// be set to 0; 0 otherwise.
static int server_sigusr_check() {
	if (server.sigusr1_flag != 0) {
		server.sigusr1_flag = 0;
		return 1;
	}
	return 0;
}

void sigusr1_handler() {
	signal(SIGUSR1, sigusr1_handler);
	server.sigusr1_flag = 1;
}

int server_init(const char* reg_pipe_path) {
	if (server_initialized()) {
		fprintf(stderr, "Error initializing server: already initialized\n");
		return 1;
	}

	if (MAX_SESSION_COUNT <= 0) {
		fprintf(stderr, "Error initializing server: cannot have a maximum of only 0 sessions\n");
		return 1;
	}

	server.sigusr1_flag = 0;
	server.reg_pipe_path = reg_pipe_path;
	server.queue = session_queue_init(SESSION_QUEUE_SIZE);
	if (!server.queue) {
		fprintf(stderr, "Error initializing server: failed to allocate session queue\n");
		return 1;
	}

	if (mkfifo(reg_pipe_path, 0666) != 0) { // make register fifo (rw-rw-rw)
		if (errno == EEXIST)
			fprintf(stderr, "Error initializing server: register fifo already in use\n");
		else
			fprintf(stderr, "Error initializing server: failed to create register fifo\n");
		session_queue_free(server.queue);
		return 1;
	}

	for (int i = 0; i < MAX_SESSION_COUNT; i++) { // Initialize workers
		int* worker_id = (int*) malloc(sizeof(int));
		*worker_id = i;
		if(pthread_create(&server.workers[i], NULL, worker_loop, worker_id) != 0) {
			fprintf(stderr, "Failed to initialize worker threads\n");
			unlink(reg_pipe_path);
			exit(EXIT_FAILURE);
		}
	}

	if ((server.reg_fd_read = open(reg_pipe_path, O_RDWR)) < 0) {
		fprintf(stderr, "Error initializing server: failed to open register fifo\n");
		unlink(reg_pipe_path);
		session_queue_close(server.queue);
		session_queue_free(server.queue);
		return 1;
	}

	return 0;
}

int server_free() {
	int failed = 0;
	if (!server_initialized()) {
		fprintf(stderr, "Error closing server: server not initialized\n");
		return 1;
	}
	if (close(server.reg_fd_read) != 0) {
		fprintf(stderr, "Error closing server: failed to close register pipe (read)\n");
		failed = 1;
	}
	if (unlink(server.reg_pipe_path) != 0) {
		fprintf(stderr, "Error closing server: failed to unlink register pipe\n");
		failed = 1;
	}
	if (session_queue_close(server.queue) != 0) {
		fprintf(stderr, "Error closing server: session queue failed to close\n");
		return 1; // cannot risk joining threads after this
	}
	for (size_t i = 0; i < MAX_SESSION_COUNT; i++) {
		void* ignore_ret;
		failed |= pthread_join(server.workers[i], &ignore_ret);
	}
	failed |= session_queue_free(server.queue);
	server.sigusr1_flag = 0; // sets the state of the server
	server.reg_fd_read = -1; // to uninitialized
	server.reg_pipe_path = NULL;
	server.queue = NULL;
	return failed;
}

/// Registers a new client (pushes the session onto the session queue).
static void register_client() {
	char req_path[MAX_CLIENT_PIPE_SIZE + 1];
	char res_path[MAX_CLIENT_PIPE_SIZE + 1];
	if (parse_setup_request(server.reg_fd_read, req_path, res_path) != 0) {
		fprintf(stderr, "Error parsing setup request\n");
		return;
	}
	Session* session = session_create(req_path, res_path);
	if (!session) {
		fprintf(stderr, "Error allocating new session\n");
		return;
	}
	if (session_queue_push(server.queue, session) != 0) {
		session_free(session);
		fprintf(stderr, "Error pushing session to session queue\n");
		return;
	}
}

int host_loop() {
	if (!server_initialized()) {
		fprintf(stderr, "Error starting host loop: server not initialized\n");
		return 1;
	}
	while (1) {
		OPCode op_code = -1;
		while (1) { // specialized non-blocking read
			char op = 0;
			if (server_sigusr_check() && ems_display_events(STDOUT_FILENO) != 0)
				fprintf(stderr, "Failed to display ems\n");
			ssize_t read_bytes = read(server.reg_fd_read, &op, sizeof(char));
			if (read_bytes < 0) {
				if (errno == EINTR && server_sigusr_check()) { // in case of SIGUSR1
					if (ems_display_events(STDOUT_FILENO) != 0) // shows the state
						fprintf(stderr, "Failed to display ems\n"); // of ems
				} else {
					fprintf(stderr, "Failed to read from register fifo\n");
				}
				continue;
			}
			if (op < OP_MIN || op > OP_MAX) {
				fprintf(stderr, "Illegal OP Code read in register fifo\n");
				continue;
			}
			op_code = (OPCode) op;
			break;
		}
		if (op_code != OP_SETUP) {
			fprintf(stderr, "Invalid OP Code read in register fifo\n");
			continue;
		}
		register_client();
	}
}
