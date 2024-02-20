#include "file.h"
#include "writer.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define IN_EXT "jobs"
#define OUT_EXT "out"

/// Standard way to write an error to a file
/// Will not write if err_fd is not a valid file descriptor or err_msg
/// is NULL.
/// @param err_fd file descriptor of error output
/// @param err_msg error to write
static void write_error(int err_fd, const char* err_msg) {
	if (err_fd < 0 || (err_msg == NULL))
		return;
	print_str(err_fd, err_msg);
}

/// Takes a filename with or without an extension and appends ext to it
/// Overwrites existing extension, for instance:
/// 	append_extension("example.txt", "out") -> "example.out"
/// @param filename to append the extension to
/// @param ext extension to append
/// @return allocated string with filename + ext as show. NULL if allocation
/// failed. Must be freed.
static char* append_extension(const char* filename, const char* ext) {
	char* full_path = NULL;
	int name_size = (int) strlen(filename);
	int ext_size = (int) strlen(ext);
	int old_ext_size = 0;
	int add_dot = 0;
	for (int i = name_size - 1; i >= 0 && filename[i] != '.'; i--, old_ext_size++);
	if (old_ext_size == name_size) { // this means there is no extension
		old_ext_size = 0;
		add_dot = 1;
	}
	full_path = (char*) malloc(sizeof(char) * ((size_t) (name_size - old_ext_size + add_dot + ext_size + 1)));
	if (full_path == NULL)
		return NULL;
	strncpy(full_path, filename, (size_t) (name_size - old_ext_size));
	full_path[name_size - old_ext_size] = '\0';	
	if (add_dot) {
		strcat(full_path, ".");
	}

	strncat(full_path, ext, (size_t) ext_size);
	return full_path;
}

int compare_extensions(const char* path, const char* ext) {
	if (path == NULL || ext == NULL)
		return 0;
	int path_size = (int) strlen(path);
	int ext_size = (int) strlen(ext);
	int path_ext_size = 0;
	for (int i = path_size - 1; i >= 0 && path[i] != '.'; i--, path_ext_size++);
	if (path_ext_size == path_size || path_ext_size != ext_size) 
		return 0; // this means path has no extension
	int offset = path_size - path_ext_size;
	for (int i = 0; (i < ext_size) && (i + offset < path_size); i++)
		if (path[i + offset] != ext[i])
			return 0;
	return 1;
}

int open_with_extension_mode(const char* filename, const char* ext,
							 int flags, mode_t mode, int err_fd,
							 const char* err_msg) {
	char* full_path = append_extension(filename, ext);
	int fd = -1;
	if (full_path == NULL) {
		write_error(err_fd, err_msg);
		return -1;
	}
	if ((fd = open(full_path, flags, mode)) == -1) {
		write_error(err_fd, err_msg);
		return -1;
	}
	free(full_path);
	return fd;
}

int open_with_extension(const char* filename, const char* ext,
							 int flags, int err_fd, const char* err_msg) {
	char* full_path = append_extension(filename, ext);
	int fd = -1;
	if (full_path == NULL) {
		write_error(err_fd, err_msg);
		return -1;
	}
	if ((fd = open(full_path, flags)) == -1) {
		write_error(err_fd, err_msg);
		return -1;
	}
	free(full_path);
	return fd;
}

int safe_close(int fd, int err_fd, const char* err_msg) {
	int result = close(fd);
	if (result == -1)
		write_error(err_fd, err_msg);
	return result;
}
