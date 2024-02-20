#include "handle_sync_failure.h"
#include "writer.h"
#include <unistd.h>
#include <stdlib.h>

void handle_lock_failure(int lock_result) {
	if (lock_result != 0) {
		print_str(STDERR_FILENO, "FATAL: Failed to lock an unlocked mutex/rwlock\n");
		exit(EXIT_FAILURE);
	}
}

void handle_unlock_failure(int unlock_result) {
	if (unlock_result != 0) {
		print_str(STDERR_FILENO, "FATAL: Failed to unlock a locked mutex/rwlock\n");
		exit(EXIT_FAILURE);
	}
}
