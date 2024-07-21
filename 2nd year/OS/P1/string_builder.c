#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "writer.h"
#include "string_builder.h"

#define INITIAL_STRING_CAPACITY 1

String* string_create(size_t reserve) {
    size_t initial_cap = (reserve == 0) ? INITIAL_STRING_CAPACITY : reserve;
	String* string = (String*) malloc(sizeof(String));
	if (!string)
    	return NULL;
	string->data = (char*) malloc(sizeof(char) * initial_cap); 
	if (!string->data) {
    	free(string);
    	return NULL;
	}
	string->size = 1;
    string->capacity = initial_cap;
	string->data[0] = '\0';
	return string;
}

String* string_append(String* string, const char* array) {
	if (!string || !array)
		return NULL;
    size_t array_size = strlen(array);
	size_t old_capacity = string->capacity;
	while(string->size + array_size >= string->capacity) {
        string->capacity *= 2;
	}
	if (string->capacity != old_capacity) {
		char* realloced = (char*) realloc(string->data, string->capacity);
		if (!realloced) {
			string_free(string);
			return NULL;
		} else {
			string->data = realloced;
		}
	}
	strncat(string->data, array, array_size);
    string->size += array_size;
	string->data[string->size] = '\0';
    return string;
}

const char* string_view(String* string) {
	if (!string)
		return NULL;
	return string->data;
}

void string_free(String* string) {
	if (!string)
		return;
    free(string->data);
    free(string);
}
