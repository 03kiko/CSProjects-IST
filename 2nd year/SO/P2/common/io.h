#ifndef COMMON_IO_H
#define COMMON_IO_H

#include <stddef.h>

/// Parses an unsigned integer from the given file descriptor.
/// @param fd The file descriptor to read from.
/// @param value Pointer to the variable to store the value in.
/// @param next Pointer to the variable to store the next character in.
/// @return 0 if the integer was read successfully, 1 otherwise.
int parse_uint(int fd, unsigned int *value, char *next);

/// Reads an unsigned integer from the given file descriptor.
/// @Note reads literally, which means no conversions are made like
/// converting "123" into it's correct unsigned integer representation.
/// @param fd to read from.
/// @param value to store the integer in.
/// @return 1 on failure; 0 otherwise.
int read_uint(int fd, unsigned int* value);

/// Reads a size_t array from the given file descriptor.
/// @Note reads literally, which mean "123" would not be converted
/// to it's appropriate size_t representation.
/// @param fd to read from.
/// @param buf to read into.
/// @param buf_size of the array.
/// @return 1 on failure; 0 otherwise.
int read_size_t_array(int fd, size_t* buf, size_t buf_size);

/// Reads a size_t from the given file descriptor.
/// @Note reads literally, which means no conversions are made like
/// converting "123" into it's correct size_t representation.
/// @param fd to read from.
/// @param value to store the size_t in.
/// @return 1 on failure; 0 otherwise.
int read_size_t(int fd, size_t* value);

/// Reads an integer from the given file descriptor.
/// @Note reads literally, which means no conversions are made like
/// converting "123" into it's correct integer representation.
/// @param fd to read from.
/// @param value to store the integer in.
/// @return 1 on failure; 0 otherwise.
int read_int(int fd, int* value);

/// Reads a string of chars from the given file descriptor.
/// @Note reads exactly buf_size bytes, no more, no less.
/// @param fd to read from.
/// @param buf to store the chars in.
/// @param buf_size how much to read into the buffer.
/// @return 1 on failure; 0 otherwise.
int read_str(int fd, char* buf, size_t buf_size);

/// Reads a string of chars from the given file descriptor.
/// @param fd to read from.
/// @param ch to read into.
/// @return 1 on failure; 0 otherwise.
int read_char(int fd, char* ch);

/// Writes length items of data from str to the given file descriptor
/// @param fd The file descriptor to write to
/// @param str The string to write
/// @param length Number of items
/// @return 0 if the string was writter successfully; 1 otherwise
int write_str(int fd, char const * str, size_t length);

/// Writes a char to the given file descriptor
/// @Note This will not write in human-readable-format
/// @param fd The file descriptor to write to
/// @param c to write
/// @return 0 if the char was written successfully, 1 otherswise
int write_char(int fd, char c);

/// Writes a signed integer to the given file descriptor.
/// @Note a write is not a print. This will write the integer
/// byte by byte, not in an human-readable format.
/// @param fd to write to.
/// @param value to write.
/// @return 0 if the integer was writter successfully, 1 otherwise.
int write_int(int fd, int value);

/// Writes an unsigned integer to the given file descriptor.
/// @Note a write is not a print. This will write the integer
/// byte by byte, not in an human-readable format.
/// @param fd to write to.
/// @param value to write.
/// @return 0 if the integer was written successfully, 1 otherwise.
int write_uint(int fd, unsigned int value);

/// Writes a size_t to the given file descriptor.
/// @Note a write is not a print. This will write the value 
/// byte by byte, not in an human-readable format.
/// @param fd to write to.
/// @param value to write.
/// @return 0 if the size_t was writter successfully, 1 otherwise.
int write_size_t(int fd, size_t value);

/// Writes a size_t array to the given file descriptor
/// @Note this will not print the elements in human-readable format
/// @param fd to write to
/// @param buf to write
/// @param buf_size size of the buffer
/// @return 1 on failure; 0 otherwise.
int write_size_t_array(int fd, size_t* buf, size_t buf_size);

/// Prints an unsigned integer to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param value The value to write.
/// @return 0 if the integer was written successfully, 1 otherwise.
int print_uint(int fd, unsigned int value);

/// Writes a string to the given file descriptor.
/// @param fd The file descriptor to write to.
/// @param str The string to write.
/// @return 0 if the string was written successfully, 1 otherwise.
int print_str(int fd, const char *str);

/// Writes a new line to the file descriptor
/// @param fd the file descriptor to write to
/// @return 1 if failed; 0 otherwise
int add_line(int fd);

#endif  // COMMON_IO_H
