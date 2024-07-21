#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "session.h"

/// SIGUSR1 function handler
void sigusr1_handler();

/// Initializes the server.
/// @param reg_pipe_path path to the register fifo.
/// @return 1 on failure; 0 otherwise.
int server_init(const char* reg_pipe_path);

/// Closes the server.
/// @return 1 on failure; 0 otherwise.
int server_free();

/// Main host loop. Keeps reading from the pipe and registering
/// new clients.
/// @Note does not close the server after ending.
/// @return 1 on failure; 0 otherwise.
int host_loop();

#endif // SERVER_STATE_H
