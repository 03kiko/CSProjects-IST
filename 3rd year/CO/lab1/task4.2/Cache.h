#ifndef CACHE_H
#define CACHE_H

#define WORD_SIZE 4                 // in bytes, i.e 32 bit words
#define BLOCK_SIZE (16 * WORD_SIZE)    // in bytes
#define DRAM_SIZE (1024 * BLOCK_SIZE) // in bytes
#define L1_SIZE (256 * BLOCK_SIZE)      // in bytes
#define L2_SIZE (512 * BLOCK_SIZE)    // in bytes

#define MODE_READ 1
#define MODE_WRITE 0

#define DRAM_READ_TIME 100
#define DRAM_WRITE_TIME 50
#define L2_READ_TIME 10
#define L2_WRITE_TIME 5
#define L1_READ_TIME 1
#define L1_WRITE_TIME 1

/* Notes:
 * blocks with 16 words => 4 bits of the word reserved for block offset
 * L1 has 256 blocks => 8 bits of the word reserved for L1 cache offset
 * L2 has 512 blocks => 9 bits reserved for L2 cache offset? (unsure)
 * DRAM has 1024 blocks => 10 bits reserved for DRAM access? (also unsure)
 */

#endif
