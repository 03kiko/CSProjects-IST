#include "eventlist.h"
#include "handle_sync_failure.h"

#include <stdlib.h>

int event_free(struct Event* event) {
	int failed = 0;
	if (!event)
		return 1;
	free(event->data);
	for (size_t i = 0; i < event->rows * event->cols; i++)
		failed |= (pthread_rwlock_destroy(event->data_locks + i) != 0);
	failed |= pthread_mutex_destroy(&event->event_lock);
	free(event->data_locks);
	free(event);
	return failed;
}

struct Event* event_create(unsigned int id, size_t rows, size_t cols) {
	struct Event* event = (struct Event*) malloc(sizeof(struct Event));
	size_t successful_locks = 0;
	size_t event_size = rows * cols;
	if (event == NULL)
		return NULL;
	event->id = id;
	event->reservations = 0;
	event->cols = cols;
	event->rows = rows;
	event->data = calloc(rows * cols, sizeof(unsigned int));
	if (event->data == NULL || (pthread_mutex_init(&event->event_lock, NULL) != 0)) {
		free(event);
		return NULL;
	}
	event->data_locks = (pthread_rwlock_t*) malloc(sizeof(pthread_rwlock_t) * event_size);
	if (event->data_locks == NULL) {
		pthread_mutex_destroy(&event->event_lock);
		free(event->data);
		free(event);
		return NULL;
	}
	for (; successful_locks < event_size; successful_locks++) // init locks
		if (pthread_rwlock_init(event->data_locks + successful_locks, NULL) != 0)
			break;
	if (successful_locks == event_size) // if we reached the end everything is good
		return event;
	for (size_t i = 0; i < successful_locks; i++) // destroy all successful locks
		pthread_rwlock_destroy(event->data_locks + i);
	pthread_mutex_destroy(&event->event_lock);
	free(event->data); // it doesnt matter if destroy failed, nothing we can do
	free(event->data_locks);
	free(event);
	return NULL;
}

size_t event_get_rows(const struct Event* event) {
	return event->rows;
}

size_t event_get_columns(const  struct Event* event) {
	return event->cols;
}

unsigned int event_get_id(const struct Event* event) {
	return event->id;
}

unsigned int event_increment_reservation(struct Event* event, int inc) {
	unsigned int res = 0;
	long int temp = inc;
	handle_lock_failure(pthread_mutex_lock(&event->event_lock));
	event->reservations = (unsigned int) (temp + event->reservations);
	res = event->reservations;
	handle_unlock_failure(pthread_mutex_unlock(&event->event_lock));
	return res;
}

struct EventList* create_list() {
	struct EventList* list = (struct EventList*)malloc(sizeof(struct EventList));
	if (!list)
		return NULL;
	if (pthread_rwlock_init(&list->head_lock, NULL) != 0) {
		free(list);
		return NULL;
	}
	list->head = NULL;
	list->tail = NULL;
	return list;
}

int append_to_list(struct EventList* list, struct Event* event) {
	if (!list)
		return 1;

	struct ListNode* new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
	if (!new_node)
		return 1;

	new_node->event = event;
	new_node->next = NULL;
	
	if (pthread_rwlock_init(&new_node->node_lock, NULL) != 0) {
		free(new_node);
		return 1;
	}

	// see the interaction this function has with blocking_get_event
	// on ems_create
	if (list->head == NULL) { // this function always expects the last
		list->head = new_node; // node to be locked in write mode
		list->tail = new_node; // so there is no need to lock here
		handle_unlock_failure(pthread_rwlock_unlock(&list->head_lock));
	} else {
		pthread_rwlock_t* old_lock = &list->tail->node_lock;
		list->tail->next = new_node;
		list->tail = new_node;
		handle_unlock_failure(pthread_rwlock_unlock(old_lock));
	}
	return 0;
}

int free_list(struct EventList* list) {
	if (!list)
		return 1;
	int failed = 0;
	struct ListNode* current = list->head;
	while (current) { // this function should ONLY be executed AFTER joining
		struct ListNode* temp = current;
		current = current->next;

		failed |= event_free(temp->event);
		failed |= pthread_rwlock_destroy(&temp->node_lock) != 0;
		free(temp);
	}
	failed |= pthread_rwlock_destroy(&list->head_lock) != 0;
	free(list);
	return failed;
}

struct Event* get_event(struct EventList* list, unsigned int event_id) {
	if (!list)
		return NULL;

	handle_lock_failure(pthread_rwlock_rdlock(&list->head_lock));
	struct ListNode* current = list->head;
	handle_unlock_failure(pthread_rwlock_unlock(&list->head_lock));
	while (current) {
		struct Event* event = current->event;
		struct ListNode* old = current;
		if (event_get_id(event) == event_id) {
			return event;
		}
		handle_lock_failure(pthread_rwlock_rdlock(&old->node_lock));
		current = current->next;
		handle_unlock_failure(pthread_rwlock_unlock(&old->node_lock));
	}

	return NULL;
}

struct Event* blocking_get_event(struct EventList* list, unsigned int event_id, int* empty_list) {
	if (!list)
		return NULL;
	*empty_list = 1;
	handle_lock_failure(pthread_rwlock_wrlock(&list->head_lock));
	struct ListNode* current = list->head;
	if (!current)
		return NULL;
	handle_unlock_failure(pthread_rwlock_unlock(&list->head_lock));
	*empty_list = 0;
	while (current) {
		struct Event* event = current->event;
		struct ListNode* old = current;
		if (event_get_id(event) == event_id)
			return event;
		handle_lock_failure(pthread_rwlock_wrlock(&old->node_lock));
		if (current->next == NULL)
			return NULL;
		current = current->next;
		handle_unlock_failure(pthread_rwlock_unlock(&old->node_lock));
	}

	return NULL;
}
