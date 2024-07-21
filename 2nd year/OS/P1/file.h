#ifndef FILE_H
#define FILE_H

#include <sys/types.h>

#define OUT_EXT "out"
#define IN_EXT "jobs"

/// Takes a given path and an extension and checks if the extension
/// of path is equal to ext
/// @param path to compare with
/// @param ext to compare with
/// @return 1 if equal; 0 otherwise
int compare_extensions(const char* path, const char* ext);

/// Takes a given filename and opens a new file by appending ext to filename.
/// If filename contains an extension, it is ignored
/// For instance: "test.txt" and ".out" would open "test.out"
/// @param filename file to swap the extension of
/// @param ext extension to open with
/// @param flags file open flags
/// @param mode permissions of the file
/// @param err_fd where to write err_msg to (-1 if not to be used)
/// @param err_msg what to write in case of failure
/// @return file descriptor of the opened file
int open_with_extension_mode(const char* filename, const char* ext, int flags,
								mode_t mode, int err_fd, const char* err_msg);

/// Takes a given filename and opens a new file by appending ext to filename.
/// If filename contains an extension, it is ignored
/// For instance: "test.txt" and ".out" would open "test.out"
/// @param filename file to swap the extension of
/// @param ext extension to open with
/// @param flags file open flags
/// @param err_fd where to write err_msg to (-1 if not to be used)
/// @param err_msg what to write in case of failure
/// @return file descriptor of the opened file
int open_with_extension(const char* filename, const char* ext, int flags,
										int err_fd, const char* err_msg);

/// Closes the file described by fd, if close does not work fails,
/// writes err_msg to the file described by err_f
/// @param fd file to close
/// @param err_fd where to output errors (-1 if not supposed to)
/// @param err_msg error to write (NULL if not supposed to)
/// @return 0 in case of success; non zero otherwise
int safe_close(int fd, int err_fd, const char* err_msg);

#endif // FILE_H
