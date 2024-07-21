#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "writer.h"
#include "handle_sync_failure.h"

static pthread_mutex_t global_output_lock;

int global_output_lock_init() {
	return pthread_mutex_init(&global_output_lock, NULL) != 0;
}

int global_output_lock_destroy() {
	return pthread_mutex_destroy(&global_output_lock) != 0;
}

void global_output_lock_acquire() {
	handle_lock_failure(pthread_mutex_lock(&global_output_lock));
}

void global_output_lock_release() {
	handle_unlock_failure(pthread_mutex_unlock(&global_output_lock));
}

/// Prints nmemb items of data from buffer, each size bytes long,
/// to the file described by fd.
/// @param fd file descriptor
/// @param buffer where to get items from
/// @param size of each item
/// @param nmemb number of items
/// @return 1 if failed to write, 0 otherwise.
static int print(int fd, void* buffer, size_t size, size_t nmemb) {
	size_t done = 0;
	size_t to_do = size * nmemb;
	while (done < to_do) {
		ssize_t written = write(fd,  buffer + done, to_do - done);
		if (written < 0)
			return 1;
		done += (size_t) written;
	}
	return 0;
}

int print_str(int fd, const char* string) {
	return print(fd, (void*) string, sizeof(char), strlen(string));
}

char* uint_to_str(unsigned int uint, char buf[MAX_UINT_SIZE + 1]) {
	size_t count = 0;
	if (uint == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}
	for (; uint > 0; uint /= 10, count++)
		buf[MAX_UINT_SIZE - count - 1] = (char) (uint % 10) + '0';
	buf[MAX_UINT_SIZE] = '\0';
	return buf + MAX_UINT_SIZE - count;
}

int print_uint(int fd, unsigned int uint) {
	char buffer[MAX_UINT_SIZE + 1];
	return print_str(fd, uint_to_str(uint, buffer));
}

int add_line(int fd) {
	char nl = '\n';
	return print(fd, &nl, sizeof(char), 1);
}
