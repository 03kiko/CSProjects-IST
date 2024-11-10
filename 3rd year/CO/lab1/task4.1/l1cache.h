#ifndef SIMPLECACHE_H
#define SIMPLECACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

/*
 * @brief resets the time.
 */
void resetTime();

/*
 * @brief gets the current time.
 */
uint32_t getTime();

/*********************** Cache *************************/

/*
 * @brief initializes the memory hierarchy.
 */
void initCache();

typedef struct MemoryLayer {
	uint8_t* buffer; /* buffer of data */
	uint32_t blockCount; /* number of blocks */
	struct MemoryLayer* next; /* next layer in the hierarchy */
	uint32_t readTime; /* time it takes to read a block */
	uint32_t writeTime; /* time it takes to write a block */
} MemoryLayer;

typedef struct CacheLine {
  uint8_t valid; /* if the line is valid */
  uint8_t dirty; /* if the line is dirty (out of sync with main memory) */
  uint32_t tag; /* tag of the stored address */
} CacheLine;

typedef struct Cache {
	MemoryLayer layer; /* inner layer */
	CacheLine* lines; /* line buffer (one per block) */
	uint8_t associative; /* if the cache is associative or not */
} Cache;

typedef struct AssociativeCache {
	Cache cache; /* inner cache */
	uint8_t* MRUbuffer; /* most recently used block of each set */
} AssociativeCache;

/*********************** Interfaces *************************/

/*
 * @brief reads a word from memory.
 *
 * @param address, address of the word.
 * @param data, word to read into.
 */
void read(uint32_t address, uint8_t* data);

/*
 * @brief writes a word to memory.
 *
 * @param address, address of the word.
 * @param data, data to write.
 */
void write(uint32_t address, uint8_t* data);

#endif
