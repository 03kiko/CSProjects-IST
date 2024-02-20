#ifndef SESSION_H
#define SESSION_H

#include <stddef.h>
#include "../common/constants.h"

typedef struct Session Session;

typedef struct SessionQueue SessionQueue;

/// Creates a new, unopened session. This means the pipes will not be
/// open.
/// @param req_path request pipe path provided by the client.
/// @param res_path response pipe path provided by the client.
/// @return NULL on failure. Pointer to the session otherwise.
Session* session_create(const char req_path[MAX_CLIENT_PIPE_SIZE],
						const char res_path[MAX_CLIENT_PIPE_SIZE]);

/// Frees the given session while unlinking and closing the pipes.
/// @Note closes the session if it is open.
/// @param session to free.
/// @return 1 on failure; 0 otherwise.
int session_free(Session* session);

/// Opens the request pipe of the given session in RDONLY mode
/// and the response pipe in WRONLY mode.
/// @Note will fail if the session has alreaby been opened.
/// @param session to open.
/// @return 1 on failure; 0 otherwise.
int session_open(Session* session);

/// Returns the request pipe fd of the given session.
/// @param session to get the fd from.
/// @return < 0 on failure; file descriptor otherwise.
int session_request_fd(const Session* session);

/// Returns the response pipe fd of the given session.
/// @param session to get the fd from.
/// @return < 0 on failure; file descriptor otherwise.
int session_response_fd(const Session* session);

/// Initializes a session producer-consumer queue that can hold
/// capacity sessions.
/// @param capacity of the session queue.
/// @return NULL on failure. Pointer to the queue otherwise.
SessionQueue* session_queue_init(size_t capacity);

/// Closes the queue: broadcasts to all consumers and producers
/// that the queue is no longer running: prevents fetches and pushes.
/// @param queue to close.
/// @return 1 on failure (maybe queue is already closed); 0 otherwise.
int session_queue_close(SessionQueue* queue);

/// Frees the given session queue.
/// @Note will free unfetched sessions.
/// @param queue to free.
/// @return 1 on failure; 0 otherwise.
int session_queue_free(SessionQueue* queue);

/// Fetches a session from the queue.
/// @param queue to fetch from.
/// @return pointer to the session; NULL on failure.
Session* session_queue_fetch(SessionQueue* queue);

/// Adds a session to the queue.
/// @param queue to push to.
/// @param session to push.
/// @return 1 on failure; 0 otherwise.
int session_queue_push(SessionQueue* queue, Session* session);

#endif // SESSION_H
