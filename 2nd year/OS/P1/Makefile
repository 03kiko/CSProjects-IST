CC = gcc

# Para mais informações sobre as flags de warning, consulte a informação adicional no lab_ferramentas
CFLAGS = -g -std=c17 -D_POSIX_C_SOURCE=200809L \
		 -Wall -Werror -Wextra \
		 -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wnull-dereference -Wshadow -Wsign-conversion -Wswitch-enum -Wundef -Wunreachable-code -Wunused \
		 -fsanitize=undefined -fsanitize=address

ifneq ($(shell uname -s),Darwin) # if not MacOS
	CFLAGS += -fmax-errors=5
endif

all: ems

ems: main.c constants.h batch.o operations.o parser.o writer.o eventlist.o file.o string_builder.o thread_pool.o handle_sync_failure.o
	$(CC) $(CFLAGS) $(SLEEP) -o ems main.c batch.o operations.o parser.o writer.o eventlist.o file.o string_builder.o thread_pool.o handle_sync_failure.o -pthread

%.o: %.c %.h
	$(CC) $(CFLAGS) -c ${@:.o=.c}

run: ems
	@./ems $(ARGS)

clean:
	rm -f *.o ems batch_processing

cleano:
	rm -f *.o

format:
	@which clang-format >/dev/null 2>&1 || echo "Please install clang-format to run this command"
	clang-format -i *.c *.h
