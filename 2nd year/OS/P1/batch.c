#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "constants.h"
#include "operations.h"
#include "parser.h"
#include "writer.h"
#include "file.h"
#include "thread_pool.h"

typedef struct {
	int in;
	int out;
	int err;
} ExecuteCommandArgs;

/// Executes the create command
/// @param in file descriptor of the input
/// @param err file descriptor of error output
static void execute_create(int in, int err)  {
	unsigned int event_id;
	size_t num_rows, num_columns;
	if (parse_create(in, &event_id, &num_rows, &num_columns) != 0) {
		global_input_lock_release();
		global_output_lock_acquire();
		print_str(err, "Invalid command. See HELP for usage\n");
		global_output_lock_release();
		return;
	}
	global_input_lock_release();

	if (ems_create(err, event_id, num_rows, num_columns)) {
		print_str(err, "Failed to create event\n");
		global_output_lock_release();
	}
}

/// Executes the reserve command
/// @param in input file descriptor
/// @param err error output file descriptor
static void execute_reserve(int in, int err) {
	unsigned int event_id;
	size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];
	size_t num_coords = parse_reserve(in, MAX_RESERVATION_SIZE, &event_id, xs, ys);
	global_input_lock_release();
	if (num_coords == 0) {
		global_output_lock_acquire();
		print_str(err, "Invalid command. See HELP for usage\n");
		global_output_lock_release();
		return;
	}

	if (ems_reserve(err, event_id, num_coords, xs, ys)) {
		print_str(err, "Failed to reserve seats\n");
		global_output_lock_release();
	}
}

/// Executes the show command
/// @param in input file descriptor
/// @param out output file descriptor
/// @param err error output file descriptor
static void execute_show(int in, int out, int err) {
	unsigned int event_id;
	if (parse_show(in, &event_id) != 0) {
		global_input_lock_release();
		global_output_lock_acquire();
		print_str(err, "Invalid command. See HELP for usage\n");
		global_output_lock_release();
		return;
	}
	global_input_lock_release();
	if (ems_show(out, err, event_id)) {
		print_str(err, "Failed to show event\n");
		global_output_lock_release();
	}
}

/// Executes the list events command
/// @param out output file descriptor
/// @param err error output file descriptor
static void execute_list_events(int out, int err) {
	global_input_lock_release();
	if (ems_list_events(out, err)) {
		print_str(err, "Failed to list events\n");
		global_output_lock_release();
	}
}

/// Executes the wait command
/// @param in input file descriptor
/// @param err error output file descriptor
/// @param parent Threadpool that lauched the thread
static void execute_wait(int in, int err, ThreadPool* parent) {
	unsigned int delay;
	unsigned int thread_id;
	int parse_wait_return_value = parse_wait(in, &delay, &thread_id);
	global_input_lock_release();
	if (parse_wait_return_value < 0) {
		global_output_lock_acquire();
		print_str(err, "Invalid command. See HELP for usage\n");
		global_output_lock_release();
		return;
	}
	if (parse_wait_return_value > 0) {
		if (threadpool_increase_wait(parent, (size_t) thread_id, delay) != 0) {
			global_output_lock_acquire();
			print_str(err, "Invalid WAIT thread id\n");
			global_output_lock_release();
		}
		return;
	}
	size_t n_threads = parent->n_threads;
	for (size_t i = 1; i <= n_threads; i++)
		threadpool_increase_wait(parent, i, delay);
}

void* execute_command(void* arg) {
	ThreadArgWrapper* wrapper = (ThreadArgWrapper*) arg;
	ExecuteCommandArgs* args = (ExecuteCommandArgs*) thread_args_unwrap(wrapper);
	int in = args->in;
	int out = args->out;
	int err = args->err;
	size_t thread_id = thread_args_thread_id(wrapper); 
	ThreadPool* parent = thread_args_parent(wrapper);
	thread_args_free(arg); 

	while (1) {
		fsync(out);
		unsigned int delay = 0;
		if ((delay = threadpool_get_wait(parent, thread_id)) != 0) {
			ems_wait(delay);
		}
		global_input_lock_acquire();
		if (threadpool_get_exit_status(parent)) {
			global_input_lock_release();
			return NULL;
		}
		switch (get_next(in)) {
		case CMD_CREATE:
			execute_create(in, err);
			break;

		case CMD_RESERVE:
			execute_reserve(in, err);
			break;

		case CMD_SHOW:
			execute_show(in, out, err);
			break;

		case CMD_LIST_EVENTS:
			execute_list_events(out, err);
			break;

		case CMD_WAIT:
			execute_wait(in, err, parent);
			break;

		case CMD_INVALID:
			global_input_lock_release();
			global_output_lock_acquire();
			print_str(err, "Invalid command. See HELP for usage\n");
			global_output_lock_release();
			break;

		case CMD_HELP:
			global_input_lock_release();
			global_output_lock_acquire();
			print_str(out,
					  "Available commands:\n"
					  "  CREATE <event_id> <num_rows> <num_columns>\n"
					  "  RESERVE <event_id> [(<x1>,<y1>) (<x2>,<y2>) ...]\n"
					  "  SHOW <event_id>\n"
					  "  LIST\n"
					  "  WAIT <delay_ms> [thread_id]\n"
					  "  BARRIER\n"
					  "  HELP\n");
			global_output_lock_release();
			break;

		case CMD_BARRIER:
			threadpool_set_exit_status(parent, BARRIER_FOUND);
			global_input_lock_release();
			break;

		case CMD_EMPTY:
			global_input_lock_release();
			break;

		case EOC:
			threadpool_set_exit_status(parent, EOC_FOUND);
			global_input_lock_release();
			return NULL;
		}
	}
	return NULL;
}

/// Helper function to initialize all global locks.
/// @param err where to dump generic error information.
/// @return 0 if successful; 1 otherwise.
static int global_locks_init(int err) {
	if (global_input_lock_init()) {
		print_str(err, "Failed to initialize global input lock\n");
		return 1;
	}

	if (global_output_lock_init()) {
		print_str(err, "Failed to initialize global output lock\n");
		global_input_lock_destroy();
		return 1;
	}
	return 0;
}

/// Helper function to destroy all global locks.
/// @param err where to dump generic error information.
/// @return 0 if successful; 1 otherwise.
static int global_locks_destroy(int err) {
	int failed_input = global_input_lock_destroy();
	int failed_output = global_output_lock_destroy();
	if (failed_input)
		print_str(err, "Failed to destroy global input lock\n");
	if (failed_output)
		print_str(err, "Failed to destroy global output lock\n");
	return failed_input || failed_output;
}

int execute_job(const char* job_path, unsigned int max_threads, unsigned int access_delay) {
	int in = -1, out = -1, err = -1;
	mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	int flags = O_CREAT | O_WRONLY | O_TRUNC;
	err = STDERR_FILENO;
	out = open_with_extension_mode(job_path, OUT_EXT, flags, perms, err, "Failed to open/create output file\n");
	if ( out == -1) {
		return 1;
	}
	in = open_with_extension(job_path, IN_EXT, O_RDONLY, err, "Failed to open input file\n");
	if (in == -1) {
		safe_close(out, err, "Failed to close output file\n"); 
		return 1;
	}

	if (ems_init(err, access_delay)) {
		print_str(err, "Failed to initialize EMS\n");
		safe_close(in, err, "Failed to close input file\n");
		safe_close(out, err, "Failed to close output file\n");
		return 1;
	}

	if (global_locks_init(err)) {
		safe_close(in, err, "Failed to close input file\n");
		safe_close(out, err, "Failed to close output file\n");
		return 1;
	}

	int exit_status = 0;
	ExecuteCommandArgs data = {in, out, err};
	ThreadPool* tp = threadpool_init(max_threads, execute_command, (void*) &data);
	exit_status |= (tp == NULL);
	if (exit_status == 0) {
		ThreadPoolExitStatus tp_exit_status;
		exit_status |= threadpool_join(tp, &tp_exit_status);
		while (!exit_status && (tp_exit_status == BARRIER_FOUND)) {
			tp = threadpool_init(max_threads, execute_command, (void*) &data);
			exit_status |= threadpool_join(tp, &tp_exit_status);
		}
	}
	if (exit_status != 0)
		print_str(err, "Failed to successfully create or destroy threadpool\n");
	ems_terminate(err);
	exit_status |= global_locks_destroy(err);
	exit_status |= safe_close(in, err, "Failed to close input file\n");
	exit_status |= safe_close(out, err, "Failed to close output file\n");
	return exit_status;
}
