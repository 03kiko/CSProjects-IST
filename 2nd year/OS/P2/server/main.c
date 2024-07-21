#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "../common/constants.h"
#include "../server/server_state.h"
#include "operations.h"
#include "server_io.h"

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s\n <pipe_path> [delay]\n", argv[0]);
		return 1;
	}

	char* endptr;
	unsigned int state_access_delay_us = STATE_ACCESS_DELAY_US;
	if (argc == 3) {
		unsigned long int delay = strtoul(argv[2], &endptr, 10);

		if (*endptr != '\0' || delay > UINT_MAX) {
			fprintf(stderr, "Invalid delay value or value too large\n");
			return 1;
		}

		state_access_delay_us = (unsigned int)delay;
	}

	if (ems_init(state_access_delay_us)) {
		fprintf(stderr, "Failed to initialize EMS\n");
		return 1;
	}

	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGPIPE);
	if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
		fprintf(stderr, "Failed to block SIGPIPE\n");
		return 1;
	}
	if(signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
		fprintf(stderr, "Failed to set SIGUSR1 signal handler function\n");
		return 1;
	}

	if (server_init(argv[1])) {
		fprintf(stderr, "Failed to initialize server\n");
		return 1;
	}

	host_loop();
	server_free();
	ems_terminate();
	return 0;
}
