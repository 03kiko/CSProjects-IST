#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include "writer.h"
#include "file.h"
#include "constants.h"
#include "batch.h"

typedef struct {
	char* job_path;
	unsigned int max_proc;
	unsigned int max_threads;
	unsigned int access_delay;
} BatchArguments;

/// Initializes the default arguments
/// @return default initialized BatchArguments
static void batch_args_init(BatchArguments* to_init) {
	if (!to_init)
		return;
	to_init->job_path = NULL;
	to_init->max_proc = 0;
	to_init->max_threads = 0;
	to_init->access_delay = STATE_ACCESS_DELAY_MS;
}

static void batch_args_copy(BatchArguments* dest, const BatchArguments* source) {
	if ((!dest) || (!source))
		return;
	dest->access_delay = source->access_delay;
	dest->job_path = source->job_path;
	dest->max_proc = source->max_proc;
	dest->max_threads = source->max_threads;
}

/// Concatenates two strings and a char in the following manner:
/// 	left + middle + right
/// @param left left string to concat
/// @param middle joining char
/// @param right right string to concat
/// @return NULL in case of failure; allocated string otherwise
static char* triple_concat(const char* left, char middle, const char* right) {
	if (left == NULL || right == NULL)
		return NULL;
	size_t left_size = strlen(left);
	size_t right_size = strlen(right);
	char* full = (char*) malloc(sizeof(char) * (left_size + right_size + 2));
	if (full == NULL)
		return NULL;
	full = strcpy(full, left);
	full[left_size] = middle;
	full[left_size + 1] = '\0';
	full = strcat(full, right);
	return full;
}

/// Checks if the given string represents a valid unsigned integer
/// @param uint_str string to parse
/// @return 1 in case of failure; 0 otherwise
static int str_to_uint(unsigned int* res, const char* uint_str, unsigned int min, unsigned int max) {
	if (uint_str == NULL)
		return 1;
	char* endptr = NULL;
	unsigned long uint_read = strtoul(uint_str, &endptr, 10);
	if ((*endptr != '\0') || (uint_read > max) || (uint_read < min))
		return 1;
	*res = (unsigned int) uint_read;
	return 0;
}

/// Handles the command line arguments expected in the following manner:
/// 	[<access delay>] <.jobs directory> <max processes allowed> <max threads allowed>
/// @param argc argument count
/// @param argv argument vector
/// @param batch_args struct to store arguments
/// @param max_proc integer to store max processes in
/// @return 1 in case of failure; 0 otherwise
static int cli_argument_handler(int argc, char** argv, BatchArguments* batch_args) {
	if (argc != 4 && argc != 5) {
		print_str(STDERR_FILENO,
			"Failed to read command line arguments, use as follows:\n"
			"\t<.jobs directory> <max processes allowed> <max threads allowed> <access delay> (optional)\n");
		return 1;
	}
	if (argc == 5) {
		if (str_to_uint(&batch_args->access_delay, argv[4], 0, UINT_MAX) != 0) {
			print_str(STDERR_FILENO, "Invalid delay value or value too large\n");
			return 1;
		}
	}
	batch_args->job_path = argv[1];
	if (str_to_uint(&batch_args->max_proc, argv[2], 1, UINT_MAX) != 0) {
		print_str(STDERR_FILENO, "Invalid max processes value (min is 1) or value too large\n");
		return 1;
	}
	if (str_to_uint(&batch_args->max_threads, argv[3], 1, UINT_MAX) != 0) {
		print_str(STDERR_FILENO, "Invalid max threads value (min is 1) or value too large\n");
		return 1;
	}
	return 0;
}

/// Standard procedure to wait for a child process
/// Writes to the STDERR if waiting fails, else writes
/// the exit status to the STDOUT
/// @param proc_count current process count
/// @return new process count after waiting
static unsigned int wait_child_processes(unsigned int proc_count) {
	int child_exit_status;
	int pid = wait(&child_exit_status);
	if ((pid > 0) && WIFEXITED(child_exit_status)) {
		print_uint(STDOUT_FILENO, (unsigned int) pid);
		print_str(STDOUT_FILENO, " exited with ");
		print_uint(STDOUT_FILENO, (unsigned int) WEXITSTATUS(child_exit_status));
		add_line(STDOUT_FILENO);
	} else {
		print_str(STDERR_FILENO, "Failed to wait for child or it did not terminate normally\n");
	}
	return proc_count - 1;
}

/// Dispatches a child process to work on a specific .jobs file.
/// @param batch_args struct to store arguments.
/// @param job name .jobs file.
/// @param batch_exec path to the batch executable.
/// @param dir so the child can close it.
/// @return pid of the child process; -1 in case of failure.
static int dispatch(const BatchArguments* batch_args, const char* job, DIR* dir) {
	BatchArguments job_args;
	batch_args_copy(&job_args, batch_args);
	job_args.job_path = triple_concat(batch_args->job_path, '/', job);
	if (job_args.job_path == NULL) {
		print_str(STDERR_FILENO, "Failed to allocate job path\n");
		return 1;
	}
	pid_t child = fork();
	if (child > 0) { // parent
		free(job_args.job_path);
		return 0;
	}
	if (child == 0) { // child
		//
		int exit_status = execute_job(job_args.job_path, job_args.max_threads, job_args.access_delay);
		free(job_args.job_path);
		if (closedir(dir) == -1) {
			print_str(STDERR_FILENO, "Child failed to close it's copy of the job directory\n");
			exit_status = 1;
		}
		exit(exit_status);
	} // error forking
	free(job_args.job_path);
	print_str(STDERR_FILENO, "Failed to create child process\n");
	return 1;
}

/// Executes all .jobs files in the specified directory
/// @param batch_args struct containing batch arguments
/// @return 1 in case of failure; 0 otherwise
static int process_batch(const BatchArguments* batch_args) {
	unsigned int proc_count = 0;
	unsigned int max_proc = batch_args->max_proc;
	DIR* job_dir = opendir(batch_args->job_path);
	if (job_dir == NULL) {
		print_str(STDOUT_FILENO, "Failed to open the job directory\n");
		return 1;
	}
	struct dirent* entry = NULL;
	while ((entry = readdir(job_dir)) != NULL) {
		if ((strcmp(entry->d_name, ".") == 0)  ||
			(strcmp(entry->d_name, "..") == 0) ||
			!compare_extensions(entry->d_name, IN_EXT))
				continue;
		if (proc_count >= max_proc)
			proc_count = wait_child_processes(proc_count);
		if (dispatch(batch_args, entry->d_name, job_dir) != 0) {
			print_str(STDERR_FILENO, "Failed to dispatch for \n");
			print_str(STDERR_FILENO, entry->d_name);
			add_line(STDERR_FILENO);
			continue;
		}
		proc_count++;
	}
	while (proc_count > 0)
		proc_count = wait_child_processes(proc_count);
	if (closedir(job_dir) == -1) {
		print_str(STDOUT_FILENO, "Failed to close the job directory\n");
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	BatchArguments cli_args;
	batch_args_init(&cli_args);
	if (cli_argument_handler(argc, argv, &cli_args))
		return 1;
	process_batch(&cli_args);
	return 0;
}
