#include "io.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int parse_uint(int fd, unsigned int *value, char *next) {
	char buf[16];

	int i = 0;
	while (1) {
		ssize_t read_bytes = read(fd, buf + i, 1);
		if (read_bytes == -1) {
			return 1;
		} else if (read_bytes == 0) {
			*next = '\0';
			break;
		}

		*next = buf[i];

		if (buf[i] > '9' || buf[i] < '0') {
			buf[i] = '\0';
			break;
		}

		i++;
	}

	unsigned long ul = strtoul(buf, NULL, 10);

	if (ul > UINT_MAX) {
		return 1;
	}

	*value = (unsigned int)ul;

	return 0;
}

/// Reads count bytes into buffer from the file described by fd.
/// @Note does not perform any sort of conversion like turning
/// "123" into it's correct integer representation.
/// @param fd file descriptor to read from.
/// @param buffer to read into.
/// @param count bytes to read.
/// @return 0 on success; 1 otherwise.
static int read_literal(int fd, void* buffer, size_t count) {
	size_t i = 0;
	while (i < count) {
		ssize_t read_bytes = read(fd, buffer + i, count - i);
		if (read_bytes < 0) { // failed to read?
			if (errno != EINTR) // if not because of a signal
				return 1; // then return error
			continue; // else continue as normal
		}
		if (read_bytes == 0) // if EOF was reached, this means
			return 1; // the writer closed their side of the pipe
		i += (size_t) read_bytes;
	}
	return 0;
}

int read_uint(int fd, unsigned int* value) {
	return read_literal(fd, (void*) value, sizeof(unsigned int));
}

int read_size_t_array(int fd, size_t* buf, size_t buf_size) {
	return read_literal(fd, (void*) buf, sizeof(size_t) * buf_size);
}

int read_size_t(int fd, size_t* value) {
	return read_literal(fd, (void*) value, sizeof(size_t));
}

int read_int(int fd, int* value) {
	return read_literal(fd, (void*) value, sizeof(int));
}

int read_str(int fd, char* buf, size_t buf_size) {
	return read_literal(fd, (void*) buf, buf_size);
}

int read_char(int fd, char* ch) {
	return read_str(fd, ch, sizeof(char));
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
		if (written < 0) { // if failed to write
			if (errno != EINTR) // and not because of a signal occuring
				return 1; // (not counting SIGPIPE), then fail
			continue; // otherwise, continue
		}
		done += (size_t) written;
	}
	return 0;
}

/// Transforms an unsigned integer to a string.
/// @param uint to transform.
/// @param buf where to the result.
/// @return pointer to the result.
static char* uint_to_str(unsigned int uint, char buf[11]) {
	size_t count = 0;
	if (uint == 0) {
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}
	for (; uint > 0; uint /= 10, count++)
		buf[10 - count - 1] = (char) (uint % 10) + '0';
	buf[10] = '\0';
	return buf + 10 - count;
}

int write_str(int fd, char const * str, size_t length) {
	return print(fd, (void*) str, sizeof(char), length);
}

int write_char(int fd, char c) {
	return print(fd, &c, sizeof(char), sizeof(char));
}

int write_int(int fd, int value) {
	return print(fd, &value, 1, sizeof(int)); 
}

int write_uint(int fd, unsigned int value) {
	return print(fd, &value, 1, sizeof(unsigned int)); 
}

int write_size_t(int fd, size_t value) {
	return print(fd, &value, 1, sizeof(size_t)); 
}

int write_size_t_array(int fd, size_t* buf, size_t buf_size) {
	return print(fd, buf, sizeof(size_t), buf_size);
}

int print_str(int fd, const char* str) {
	return print(fd, (void*) str, sizeof(char), strlen(str));
}

int print_uint(int fd, unsigned int value) {
	char buffer[11];
	return print_str(fd, uint_to_str(value, buffer));
}

int add_line(int fd) {
	char nl = '\n';
	return print(fd, &nl, sizeof(char), 1);
}
