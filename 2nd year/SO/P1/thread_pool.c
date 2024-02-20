#include <stdlib.h>
#include "thread_pool.h"
#include "writer.h"
#include "handle_sync_failure.h"

void thread_args_free(ThreadArgWrapper* thread_args) {
	if (!thread_args)
		return;
	free(thread_args);
	thread_args = NULL;
}

void* thread_args_unwrap(ThreadArgWrapper* thread_args) {
	return thread_args->args;
}

size_t thread_args_thread_id(ThreadArgWrapper* thread_args) {
	return thread_args->thread_id;
}

ThreadPool* thread_args_parent(ThreadArgWrapper* thread_args) {
	return thread_args->parent;
}

/// Wraps the function a given thread calls.
/// Waits for the threadpool to finish initializing before starting
/// the function itself.
/// If the threadpool sets the exit_status to anything besides CONTINUE
/// the thread will exit early.
/// @param wrapper where the function is stored
/// @return NULL in case of early exit; otherwise returns the
/// thread function return value.
static void* thread_wrapper_fn(void* wrapper) {
	ThreadArgWrapper* arg_wrapper = (ThreadArgWrapper*) wrapper;
	if (threadpool_get_exit_status(thread_args_parent(arg_wrapper))) {
		thread_args_free(wrapper);
		return NULL;
	}
	return arg_wrapper->threadfn(wrapper);
}

/// Creates a new thread that runs threadfn.
/// @param thread where to store the new thread.
/// @param threadfn function to run.
/// @param args to run threadfn with.
/// @return 0 if successful; 1 otherwise.
static int thread_init(Thread* thread, void* args) {
	thread->wait_time = 0;
	if (pthread_mutex_init(&thread->mutex, NULL) != 0) {
		thread_args_free((ThreadArgWrapper*) args);
		return 1;
	}
	if (pthread_create(&thread->tid, NULL, thread_wrapper_fn, args) != 0) {
		pthread_mutex_destroy(&thread->mutex);
		thread_args_free((ThreadArgWrapper*) args);
		return 1;
	}
	return 0;
}

/// Waits for all threads in the pool and frees it
/// @param threadpool to free
/// @param exit_status  where to store the exit status of the thread pool 
/// @param n_threads number of threads to join
/// @return 0 on success; 1 otherwise.
static int threadpool_join_nthreads(ThreadPool* threadpool,
	ThreadPoolExitStatus* exit_status, size_t n_threads) {
	int failed = 0;
	if (!threadpool)
		return 1;
	for (size_t i = 0; i < n_threads; i++) {
		void* ignore_result = NULL;
		failed |= pthread_join(threadpool->threads[i].tid, &ignore_result) != 0;
	}
	for (size_t i = 0; i < n_threads; i++)
		failed |= pthread_mutex_destroy(&threadpool->threads[i].mutex) != 0;
	failed |= pthread_mutex_destroy(&threadpool->pool_lock);
	*exit_status = threadpool->exit_status;
	free(threadpool->threads);
	free(threadpool);
	threadpool = NULL;
	return failed;
}

ThreadPool* threadpool_init(size_t n_threads, void* (*threadfn)(void*), void* args) {
    ThreadPool* new_pool = (ThreadPool*) malloc(sizeof(ThreadPool));
    if(!new_pool)
		return NULL;
	new_pool->threads = (Thread*) malloc(sizeof(Thread) * n_threads);
	if (!new_pool->threads) {
		free(new_pool);
		return NULL;
	}
	if (pthread_mutex_init(&new_pool->pool_lock, NULL) != 0) {
		free(new_pool->threads);
		free(new_pool);
		return NULL;
	}
	handle_lock_failure(pthread_mutex_lock(&new_pool->pool_lock));
    new_pool->n_threads = n_threads;
    new_pool->exit_status = CONTINUE;
	size_t i = 0;
	for (; i < n_threads; i++) {
		ThreadArgWrapper* wrapper = (ThreadArgWrapper*) malloc(sizeof(ThreadArgWrapper));
		if (!wrapper) {
			new_pool->exit_status = FATAL_ERROR;
			break;
		}
		wrapper->args = args; // user responsible for freeing wrapper
		wrapper->threadfn = threadfn;
		wrapper->thread_id = i;
		wrapper->parent = new_pool;
		if (thread_init(&new_pool->threads[i], (void*) wrapper)) {
			new_pool->exit_status = FATAL_ERROR;
			break;
		}
	}
	handle_unlock_failure(pthread_mutex_unlock(&new_pool->pool_lock));
	if (i < n_threads) {
		ThreadPoolExitStatus ignore_exit_status;
		threadpool_join_nthreads(new_pool, &ignore_exit_status, i);
		return NULL;
	}
    return new_pool;
}

int threadpool_join(ThreadPool* threadpool, ThreadPoolExitStatus* exit_status) {
	return threadpool_join_nthreads(threadpool, exit_status, threadpool->n_threads);
}

void threadpool_set_exit_status(ThreadPool* threadpool, ThreadPoolExitStatus exit_status) {
    handle_lock_failure(pthread_mutex_lock(&threadpool->pool_lock));
    threadpool->exit_status = exit_status;
    handle_unlock_failure(pthread_mutex_unlock(&threadpool->pool_lock));
}

int threadpool_get_exit_status(ThreadPool* threadpool) {
	int status;
    handle_lock_failure(pthread_mutex_lock(&threadpool->pool_lock));
	status = threadpool->exit_status != CONTINUE;
    handle_unlock_failure(pthread_mutex_unlock(&threadpool->pool_lock));
	return status;
}

int threadpool_increase_wait(ThreadPool* threadpool, size_t thread_id, unsigned int wait_time) {
	if (!threadpool || thread_id > threadpool->n_threads || thread_id < 1)
		return 1; // n_threads is constant
	thread_id--;
	handle_lock_failure(pthread_mutex_lock(&threadpool->threads[thread_id].mutex));
	threadpool->threads[thread_id].wait_time += wait_time;
	handle_unlock_failure(pthread_mutex_unlock(&threadpool->threads[thread_id].mutex));
	return 0;
}

unsigned int threadpool_get_wait(ThreadPool* threadpool, size_t thread_id) {
	unsigned int wait_time;
    handle_lock_failure(pthread_mutex_lock(&threadpool->threads[thread_id].mutex));
    wait_time = threadpool->threads[thread_id].wait_time;
	threadpool->threads[thread_id].wait_time = 0;
    handle_unlock_failure(pthread_mutex_unlock(&threadpool->threads[thread_id].mutex));
    return wait_time;
}
