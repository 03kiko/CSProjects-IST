#ifndef OP_CODES_H
#define OP_CODES_H

typedef enum {
	OP_SETUP = 1,
	OP_QUIT = 2,
	OP_CREATE = 3,
	OP_RESERVE = 4,
	OP_SHOW = 5,
	OP_LIST = 6
} OPCode;

#define OP_MIN 1
#define OP_MAX 6

#endif
