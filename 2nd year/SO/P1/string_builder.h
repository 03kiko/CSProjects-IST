#ifndef STRING_BUILDER_H 
#define STRING_BUILDER_H 

#include <stddef.h>

typedef struct {
    char* data; /// Array of chars that holds the string
    size_t size; /// Current size of the string
    size_t capacity; /// Allocated capacity of the array
} String;

/// Creates a string.
/// @param reserve initial capacity, 0 for default capacity.
/// @return Pointer to the string, NULL on failure
String* string_create(size_t reserve);

/// Appends a null terminated char array to string, on the right.
/// @param string to append to.
/// @param array to append.
/// @return pointer to string after appending.
String* string_append(String* string, const char* array);

/// Views the contents of a string.
/// @param string to view content of.
/// @return pointer to the string's data.
const char* string_view(String* string);

/// Frees the memory allocated for the string.
void string_free(String* string);

#endif // STRING_BUILDER_H
