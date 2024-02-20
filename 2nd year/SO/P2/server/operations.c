#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../common/io.h"
#include "server_io.h"
#include "eventlist.h"
#include "operations.h" 

static struct EventList* event_list = NULL;
static unsigned int state_access_delay_us = 0;

/// Gets the event with the given ID from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event_id The ID of the event to get.
/// @param from First node to be searched.
/// @param to Last node to be searched.
/// @return Pointer to the event if found, NULL otherwise.
static struct Event* get_event_with_delay(unsigned int event_id, struct ListNode* from, struct ListNode* to) {
	struct timespec delay = {0, state_access_delay_us * 1000};
	nanosleep(&delay, NULL);  // Should not be removed

	return get_event(event_list, event_id, from, to);
}

/// Gets the index of a seat.
/// @note This function assumes that the seat exists.
/// @param event Event to get the seat index from.
/// @param row Row of the seat.
/// @param col Column of the seat.
/// @return Index of the seat.
static size_t seat_index(struct Event* event, size_t row, size_t col) { return (row - 1) * event->cols + col - 1; }

int ems_init(unsigned int delay_us) {
	if (event_list != NULL) {
		fprintf(stderr, "EMS state has already been initialized\n");
		return 1;
	}

	event_list = create_list();
	state_access_delay_us = delay_us;

	return event_list == NULL;
}

int ems_terminate() {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	free_list(event_list);
	event_list = NULL;
	return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_wrlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	if (get_event_with_delay(event_id, event_list->head, event_list->tail) != NULL) {
		fprintf(stderr, "Event already exists\n");
		pthread_rwlock_unlock(&event_list->rwl);
		return 1;
	}

	struct Event* event = malloc(sizeof(struct Event));

	if (event == NULL) {
		fprintf(stderr, "Error allocating memory for event\n");
		pthread_rwlock_unlock(&event_list->rwl);
		return 1;
	}

	event->id = event_id;
	event->rows = num_rows;
	event->cols = num_cols;
	event->reservations = 0;
	if (pthread_mutex_init(&event->mutex, NULL) != 0) {
		pthread_rwlock_unlock(&event_list->rwl);
		free(event);
		return 1;
	}
	event->data = calloc(num_rows * num_cols, sizeof(unsigned int));

	if (event->data == NULL) {
		fprintf(stderr, "Error allocating memory for event data\n");
		pthread_rwlock_unlock(&event_list->rwl);
		free(event);
		return 1;
	}

	if (append_to_list(event_list, event) != 0) {
		fprintf(stderr, "Error appending event to list\n");
		pthread_rwlock_unlock(&event_list->rwl);
		free(event->data);
		free(event);
		return 1;
	}

	pthread_rwlock_unlock(&event_list->rwl);
	return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id, event_list->head, event_list->tail);

	pthread_rwlock_unlock(&event_list->rwl);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		return 1;
	}

	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr, "Error locking mutex\n");
		return 1;
	}

	for (size_t i = 0; i < num_seats; i++) {
		if (xs[i] <= 0 || xs[i] > event->rows || ys[i] <= 0 || ys[i] > event->cols) {
			fprintf(stderr, "Seat out of bounds\n");
			pthread_mutex_unlock(&event->mutex);
			return 1;
		}
	}

	for (size_t i = 0; i < event->rows * event->cols; i++) {
		for (size_t j = 0; j < num_seats; j++) {
			if (seat_index(event, xs[j], ys[j]) != i) {
				continue;
			}

			if (event->data[i] != 0) {
				fprintf(stderr, "Seat already reserved\n");
				pthread_mutex_unlock(&event->mutex);
				return 1;
			}

			break;
		}
	}

	unsigned int reservation_id = ++event->reservations;

	for (size_t i = 0; i < num_seats; i++) {
		event->data[seat_index(event, xs[i], ys[i])] = reservation_id;
	}

	pthread_mutex_unlock(&event->mutex);
	return 0;
}

int ems_show(int out_fd, unsigned int event_id) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id, event_list->head, event_list->tail);

	pthread_rwlock_unlock(&event_list->rwl);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr, "Error locking mutex\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	size_t num_rows = event->rows;
	size_t num_cols = event->cols;
	if ((write_int(out_fd, 0) != 0) ||
		(write_size_t(out_fd, num_rows) != 0) ||
		(write_size_t(out_fd, num_cols) != 0)) {
		pthread_mutex_unlock(&event->mutex);
		return IO_FAILURE;
	}

	for (size_t i = 1; i <= num_rows; i++) {
		for (size_t j = 1; j <= num_cols; j++) {
			if (write_uint(out_fd, event->data[seat_index(event, i, j)]) != 0) {
				pthread_mutex_unlock(&event->mutex);
				return IO_FAILURE;
			}
		}
	}
	pthread_mutex_unlock(&event->mutex);
	return 0;
}

int ems_list_events(int out_fd) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		if (write_int(out_fd, 1) != 0)
			return IO_FAILURE;
		return 1;
	}

	struct ListNode* current = event_list->head;

	if (!current) {
		pthread_rwlock_unlock(&event_list->rwl);
		if ((write_int(out_fd, 0) != 0) ||
			(write_size_t(out_fd, 0) != 0))
			return IO_FAILURE;
		return 0;
	}

	size_t n_events = 0;
	while (current != NULL) {
		n_events++;
		current = current->next;
	}
	if ((write_int(out_fd, 0) != 0) ||
		(write_size_t(out_fd, n_events) != 0)) {
		pthread_rwlock_unlock(&event_list->rwl);
		return IO_FAILURE;
	}

	current = event_list->head;
	while (current != NULL) {
		if (write_uint(out_fd, current->event->id) != 0) {
			pthread_rwlock_unlock(&event_list->rwl);
			return IO_FAILURE;
		}
		current = current->next;
	}

	pthread_rwlock_unlock(&event_list->rwl);
	return 0;
}


static int ems_display_event_seats(int out_fd, struct Event* event) {
	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr , "Error locking mutex\n");
		return 1;
	}

	for (size_t i = 1; i <= event->rows; i++) {
		for (size_t j = 1; j <= event->cols; j++) {
			if ((print_uint(out_fd, event->data[seat_index(event, i, j)]) != 0)
				|| (print_str(out_fd, " ") != 0)) {
				pthread_mutex_unlock(&event->mutex);
				fprintf(stderr, "Failed to fully display event\n");
				return 1;
			}
		}

		if (add_line(out_fd) != 0) {
			pthread_mutex_unlock(&event->mutex);
			fprintf(stderr, "Failed to add newline while displaying seats\n");
			return 1;
		}
	}

	pthread_mutex_unlock(&event->mutex);
	return 0;
}

int ems_display_events(int out_fd) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	struct ListNode* current = event_list->head;
	if(!current) {
		pthread_rwlock_unlock(&event_list->rwl);
		print_str(out_fd, "No events to show\n");
		return 0;
	}

	int failed = 0;
	while(current != NULL) {
		struct Event* event = current->event;
		if ((print_str(out_fd, "Event: ") != 0) ||
			(print_uint(out_fd, event->id) != 0) ||
			(add_line(out_fd) != 0) ||
			(ems_display_event_seats(out_fd, event) != 0)) {
			fprintf(stderr, "Failed to display event\n");
			failed = 1;
			break;
		}
		current = current->next;
	}

	pthread_rwlock_unlock(&event_list->rwl);
	return failed;
}
