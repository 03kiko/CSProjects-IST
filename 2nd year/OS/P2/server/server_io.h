#ifndef SERVER_IO_H
#define SERVER_IO_H

#include "../common/op_codes.h"
#include <stddef.h>
#include "../common/constants.h"

#define IO_FAILURE -1

/// Parses an op code from the given file descriptor.
/// @param in the file descriptor to read from.
/// @param op_code Pointer to to store the op code in.
/// @return 1 on failure; 0 otherwise.
int get_op_code(int in, OPCode* op_code);

/// Parses a setup request from the client.
/// @param in the file descriptor to read from
/// @param request_buf request pipe path provided by the client
/// @param response_buf response pipe path provided by the clien
/// @return 1 on failure; 0 otherwise
int parse_setup_request(int in, char request_buf[MAX_CLIENT_PIPE_SIZE + 1],
						char response_buf[MAX_CLIENT_PIPE_SIZE + 1]);

/// Parses a create request from the client.
/// @param in file descriptor to read from.
/// @param event_id pointer to store the envent_id in.
/// @param num_rows buffer to store the number of rows in.
/// @param num_cols pointer to store the number of columns in.
/// @return 1 on failure; 0 otherwise.
int parse_create_request(int in, unsigned int* event_id, size_t* num_rows,
						 size_t* num_cols);

/// Parses a reserve request from the client.
/// @param in the file descriptor to read from.
/// @param event_id where to store the event id.
/// @param num_seats where to store the number of seats.
/// @param xs buffer to store the x position of each seat.
/// @param ys buffer to store the y position of each seat.
/// @return 1 on failure; 0 otherwise.
int parse_reserve_request(int in, unsigned int* event_id, size_t* num_seats,
						  size_t* xs, size_t* ys);

/// Parses a show request from the client.
/// @param in the file descriptor to read from.
/// @param event_id where to store the event id.
/// @return 1 on failure; 0 otherwise.
int parse_show_request(int in, unsigned int* event_id);

#endif // SERVER_IO_H
