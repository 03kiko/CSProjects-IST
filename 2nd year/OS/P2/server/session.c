#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#include "session.h"

struct Session {
	char request_pipe_path[MAX_CLIENT_PIPE_SIZE + 1];
	char response_pipe_path[MAX_CLIENT_PIPE_SIZE + 1];
	int request_fd;
	int response_fd;
};

struct SessionQueue {
	Session** sessions;
	int8_t running;
	size_t capacity;
	size_t count;
	size_t fetch_idx;
	size_t write_idx;
	pthread_mutex_t queue_mut;
	pthread_cond_t producer;
	pthread_cond_t consumer;
};

Session* session_create(const char req_path[MAX_CLIENT_PIPE_SIZE],
						const char res_path[MAX_CLIENT_PIPE_SIZE]) {
	if (!req_path || !res_path)
		return NULL;
	Session* new = (Session*) malloc(sizeof(Session));
	if (!new)
		return NULL;
	new->request_fd = -1;
	new->response_fd = -1;
	strncpy(new->request_pipe_path, req_path, MAX_CLIENT_PIPE_SIZE);
	strncpy(new->response_pipe_path, res_path, MAX_CLIENT_PIPE_SIZE);
	return new;
}

int session_free(Session* session) {
	if (!session)
		return 1;
	if (session->response_fd < 0 || session->request_fd < 0) {
		free(session);
		return 0;
	}
	int failed = close(session->response_fd) != 0 || close(session->request_fd) != 0;
	session->request_fd = -1;
	session->response_fd = -1;
	free(session);
	return failed;
}

int session_open(Session* session) {
	if (!session || session->response_fd >= 0 || session->request_fd >= 0)
		return 1;
	if ((session->request_fd = open(session->request_pipe_path, O_RDONLY)) < 0) {
		close(session->response_fd);
		return 1;
	}
	if ((session->response_fd = open(session->response_pipe_path, O_WRONLY)) < 0) {
		close(session->request_fd);
		return 1;
	}
	return 0;
}

int session_request_fd(const Session* session) {
	return session->request_fd;
}

int session_response_fd(const Session* session) {
	return session->response_fd;
}

SessionQueue* session_queue_init(size_t capacity) {
	if (capacity == 0) {
		fprintf(stderr, "Tried to make a queue of size 0\n");
		return NULL;
	}
	SessionQueue* queue = (SessionQueue*) malloc(sizeof(SessionQueue));
	if (!queue)
		return NULL;
	queue->sessions = (Session**) malloc(sizeof(Session*) * capacity);
	if (!queue->sessions) {
		free(queue);
		return NULL;
	}
	queue->running = 1;
	queue->write_idx = 0;
	queue->fetch_idx = 0;
	queue->count = 0;
	queue->capacity = capacity;
	int failed = 0;
	failed |= pthread_cond_init(&queue->producer, NULL) != 0; 
	failed |= pthread_cond_init(&queue->consumer, NULL) != 0; 
	failed |= pthread_mutex_init(&queue->queue_mut, NULL) != 0;
	if (failed != 0) {
		free(queue->sessions);
		free(queue);
		return NULL;
	}
	for (size_t i = 0; i < queue->capacity; i++)
		queue->sessions[i] = NULL;
	return queue;
}

int session_queue_close(SessionQueue* queue) {
	if (!queue)
		return 1;
	if (pthread_mutex_lock(&queue->queue_mut) != 0) {
		fprintf(stderr, "Failed to lock queue mutex while closing\n");
		return 1;
	}
	if (!queue->running) {
		if (pthread_mutex_unlock(&queue->queue_mut) != 0)
			fprintf(stderr, "Failed to unlock queue mutex while closing\n");
		return 1;
	}
	int failed = 0;
	queue->running = 0;
	if ((pthread_cond_broadcast(&queue->consumer) != 0) ||
		(pthread_cond_broadcast(&queue->producer) != 0)) {
		fprintf(stderr, "Failed to broadcast consumers or producers while closing\n");
		failed = 1;
	}
	if (pthread_mutex_unlock(&queue->queue_mut) != 0) {
		fprintf(stderr, "Failed to unlock queue mutex while closing\n");
		failed = 1;
	}
	return failed;
}

int session_queue_free(SessionQueue* queue) {
	if (!queue)
		return 1;
	int failed = 0;
	failed |= pthread_cond_destroy(&queue->producer) != 0;
	failed |= pthread_cond_destroy(&queue->consumer) != 0;
	failed |= pthread_mutex_destroy(&queue->queue_mut) != 0;
	for (size_t i = 0; i < queue->capacity; i++)
		if (queue->sessions[i] != NULL)
			session_free(queue->sessions[i]);
	free(queue->sessions);
	free(queue);
	queue = NULL;
	return failed;
}

Session* session_queue_fetch(SessionQueue* queue) {
	Session* fetched = NULL;
	if (!queue)
		return NULL;
	if (pthread_mutex_lock(&queue->queue_mut) != 0) {
		fprintf(stderr, "Failed to lock queue mutex before fetching\n");
		return NULL;
	}
	while (queue->count == 0 && queue->running) {
		if (pthread_cond_wait(&queue->consumer, &queue->queue_mut) != 0) {
			fprintf(stderr, "Failed to wait while fetching\n");
			if (pthread_mutex_unlock(&queue->queue_mut) != 0)
				fprintf(stderr, "Failed to unlock queue mutex after fetch error\n");
			return NULL;
		}
	}
	if (!queue->running) {
		if (pthread_mutex_unlock(&queue->queue_mut) != 0)
			fprintf(stderr, "Failed to unlock queue mutex while fetching\n");
		return NULL;
	}
	fetched = queue->sessions[queue->fetch_idx];
	queue->sessions[queue->fetch_idx] = NULL;
	queue->fetch_idx = (queue->fetch_idx + 1) % queue->capacity;
	queue->count--;
	if (pthread_cond_signal(&queue->producer) != 0)
		fprintf(stderr, "Failed to signal a producer after fetching\n");
	if (pthread_mutex_unlock(&queue->queue_mut) != 0)
		fprintf(stderr, "Failed to unlock queue mutex after fetching\n");
	return fetched;
}

int session_queue_push(SessionQueue* queue, Session* session) {
	if (!session || !queue)
		return 1;
	if (pthread_mutex_lock(&queue->queue_mut) != 0) {
		fprintf(stderr, "Failed to lock queue mutex before pushing\n");
		return 1;
	}
	while (queue->count == queue->capacity && queue->running) {
		if (pthread_cond_wait(&queue->producer, &queue->queue_mut) != 0) {
			fprintf(stderr, "Failed to wait while pushing\n");
			if (pthread_mutex_unlock(&queue->queue_mut) != 0)
				fprintf(stderr, "Failed to unlock queue mutex after push error\n");
			return 1;
		}
	}
	if (!queue->running) {
		if (pthread_mutex_unlock(&queue->queue_mut) != 0)
			fprintf(stderr, "Failed to unlock queue mutex while pushing\n");
		return 1;
	}
	queue->sessions[queue->write_idx] = session;
	queue->write_idx = (queue->write_idx + 1) % queue->capacity;
	queue->count++;
	if (pthread_cond_signal(&queue->consumer) != 0)
		fprintf(stderr, "Failed to signal consumer after pushing\n");
	if (pthread_mutex_unlock(&queue->queue_mut) != 0)
		fprintf(stderr, "Failed to unlock queue mutex after pushing\n");
	return 0;
}
