#ifndef	THREAD_POOL_H 
#define THREAD_POOL_H

#include <pthread.h>

typedef enum {
	CONTINUE,
	EOC_FOUND,
	BARRIER_FOUND,
	FATAL_ERROR
} ThreadPoolExitStatus;

typedef struct {
	pthread_t tid;
	unsigned int wait_time;
	pthread_mutex_t mutex;
} Thread;

typedef struct {
	ThreadPoolExitStatus exit_status; /// Indicates the threads to stop
    size_t n_threads; /// Number of threads
	Thread* threads; /// Threads launched by the threadpool
	pthread_mutex_t pool_lock; /// Locks the pool when initializing and other accesses
} ThreadPool;

typedef struct {
	void* args; /// Generic arguments to be passed to the function
	void* (*threadfn)(void*); /// Function the thread was called with
	size_t thread_id; /// Threadpool thread id (set on creation)
	ThreadPool* parent; /// Threadpool that launched the thread
} ThreadArgWrapper; /// Wraps the arguments to a function launched by the threadpool
/// The user is responsible for freeing the wrapper inside a thread's function.

/// Frees the thread_args struct.
/// @Note does not free the args field, that is responsibility
/// of the user.
/// @param thread_args to free.
void thread_args_free(ThreadArgWrapper* thread_args);

/// Unwraps the arguments.
/// @param thread_args to unwrap the arguments from.
/// @return void* to the args;
void* thread_args_unwrap(ThreadArgWrapper* thread_args);

/// Gets the thread id.
/// @param thread_args to get the id from.
/// @return thread id.
size_t thread_args_thread_id(ThreadArgWrapper* thread_args);

/// Gets the threadpool (parent) that lauched the thread.
/// @param thread_args to get the parent of.
/// @return pointer to the parent.
ThreadPool* thread_args_parent(ThreadArgWrapper* thread_args);

/// Initializes a threadpool with n_threads running threadfn
/// with args.
/// @param n_threads number of threads.
/// @param threadfn function to run.
/// @param args threadfn arguments
/// @return pointer to the threadpool.
ThreadPool* threadpool_init(size_t n_threads, void* (*threadfn)(void*), void* args);

/// Waits for all threads in the pool to join and frees it.
/// Puts the exit status in the exit status parameter.
/// @param threadpool to free.
/// @param exit_status where to store the exit status of the threadpool.
/// @return 0 on success; 1 otherwise.
int threadpool_join(ThreadPool* threadpool, ThreadPoolExitStatus* exit_status);

/// Sets the exit_status of the threadpool to exit_status.
/// @param threadpool to set the exit_status of.
/// @param exit_status state to set to.
void threadpool_set_exit_status(ThreadPool* threadpool, ThreadPoolExitStatus exit_status);

/// Gets the exit_status of the threadpool.
/// @param threadpool to get the exit_status from.
/// @return 0 if the state is CONTINUTE; 1 otherwise.
int threadpool_get_exit_status(ThreadPool* threadpool);

/// Increases the wait time of the thread given by thread_id.
/// @param threadpool where the thread was launched from.
/// @param thread_id id of the thread to increase the time.
/// @param wait_time to increase by.
/// @return 0 if successful; 1 otherwise (only fails if the id is invalid).
int threadpool_increase_wait(ThreadPool* threadpool, size_t thread_id, unsigned int wait_time);

/// Gets the wait time from the thread given by thread_id and sets it to 0.
/// @param threadpool where the thread was launched from.
/// @param thread_id of the thread to get the wait time of.
/// @return wait time of the thread.
unsigned int threadpool_get_wait(ThreadPool* threadpool, size_t thread_id);

#endif // THREAD_POOL_H
