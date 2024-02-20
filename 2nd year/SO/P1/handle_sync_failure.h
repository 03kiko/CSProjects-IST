#ifndef HANDLE_SYNC_FAILURE
#define HANDLE_SYNC_FAILURE

#include <pthread.h>

/// These are two utility functions used whenever a potential
/// deadlock causing / synchronization error happens (as in,
/// a locked lock/mutex failing to unlock or an unlocked
/// lock/mutex failing to lock).
/// All they do in general is try to print an error
/// to the stderr (this print is not synchronized) and then
/// just exit(EXIT_FAILURE).

/// Exits if the lock result is not zero.
/// @param lock_result to test.
void handle_lock_failure(int lock_result);

/// Exits if the unlocking result is not zero.
/// @param unlock_result to test.
void handle_unlock_failure(int unlock_result);

#endif // HANDLE_SYNC_FAILURE
