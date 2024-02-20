#include "server_io.h"
#include "../common/io.h"

int get_op_code(int in, OPCode* op_code) {
	char op;
	if (read_char(in, &op))
		return 1;
	if (op < OP_MIN || op > OP_MAX)
		return 1;
	*op_code = (OPCode) op;
	return 0;
}

int parse_setup_request(int in, char request_buf[MAX_CLIENT_PIPE_SIZE + 1],
						char response_buf[MAX_CLIENT_PIPE_SIZE + 1]) {
	if ((read_str(in, request_buf, MAX_CLIENT_PIPE_SIZE) != 0) ||
		(read_str(in, response_buf, MAX_CLIENT_PIPE_SIZE) != 0))
		return 1;
	request_buf[MAX_CLIENT_PIPE_SIZE] = '\0';
	response_buf[MAX_CLIENT_PIPE_SIZE] = '\0';
	return 0;
}

int parse_create_request(int in, unsigned int* event_id, size_t* num_rows, size_t* num_cols) {
	if (read_uint(in, event_id) != 0)
		return 1;
	if (read_size_t(in, num_rows) != 0)
		return 1;
	return read_size_t(in, num_cols) != 0;
}

int parse_reserve_request(int in, unsigned int* event_id, size_t* num_seats, size_t* xs, size_t* ys) {
	if (read_uint(in, event_id) != 0)
		return 1;
	if (read_size_t(in, num_seats) != 0)
		return 1;
	if (read_size_t_array(in, xs, *num_seats) != 0)
		return 1;
	return read_size_t_array(in, ys, *num_seats) != 0;
}

int parse_show_request(int in, unsigned int* event_id) {
	return read_uint(in, event_id) != 0;
}
