#ifndef WRITER_H
#define WRITER_H

#define MAX_UINT_SIZE 10

/// Initializes the global output lock.
/// @return 0 if successful; 1 otherwise.
int global_output_lock_init();

/// Destroys the global output lock.
/// @return 0 if successful; 1 otherwise.
int global_output_lock_destroy();

/// Locks the global output lock.
/// @return 0 if successful; 1 otherwise.
void global_output_lock_acquire();

/// Unlocks the global output lock.
/// @return 0 if successful; 1 otherwise.
void global_output_lock_release();

/// Prints a string to the file described by fd.
/// @param fd file descriptor
/// @param buffer string to print
/// @return 1 if failed to write, 0 otherwise.
int print_str(int fd, const char* string);

/// Transforms an unsigned integer to a string.
/// @param uint to transform.
/// @param buf where to the result.
/// @return pointer to the result.
char* uint_to_str(unsigned int uint, char buf[MAX_UINT_SIZE + 1]);

/// Prints an unsigned integer to the file described by fd.
/// @param fd file descriptor
/// @param uint unsigned integer to print
/// @return 1 if failed to write, 0 otherwise.
int print_uint(int fd, unsigned int uint);

/// Prints a new line to the file described by fd.
/// @param fd file descriptor.
/// @return 1 if failed to write, 0 otherwise.
int add_line(int fd);

#endif // WRITER_H
