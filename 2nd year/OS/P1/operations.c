#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "operations.h"
#include "eventlist.h"
#include "writer.h"
#include "constants.h"
#include "string_builder.h"
#include "handle_sync_failure.h"

static struct EventList* event_list = NULL;
static unsigned int state_access_delay_ms = 0;

/// Calculates a timespec from a delay in milliseconds.
/// @param delay_ms Delay in milliseconds.
/// @return Timespec with the given delay.
static struct timespec delay_to_timespec(unsigned int delay_ms) {
	return (struct timespec){delay_ms / 1000, (delay_ms % 1000) * 1000000};
}

/// Gets the event with the given ID from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event_id The ID of the event to get.
/// @return Pointer to the event if found, NULL otherwise.
static struct Event* get_event_with_delay(unsigned int event_id) {
	struct timespec delay = delay_to_timespec(state_access_delay_ms);
	nanosleep(&delay, NULL);  // Should not be removed

	return get_event(event_list, event_id);
}

/// Gets the event with the given ID from the state.
/// @Note will leave the last node it reached locked in write mode.
/// If it is empty, the head is left in write mode.
/// If the event does not exist, no node is left locked.
/// @param event_id id of the event to get.
/// @param empty_list set to 1 if the list is currently empty.
/// @return NULL if the event does not exist. Pointer to the event otherwise.
static struct Event* blocking_get_event_with_delay(unsigned int event_id, int* empty_list) {
	struct timespec delay = delay_to_timespec(state_access_delay_ms);
	nanosleep(&delay, NULL);  // Should not be removed

	return blocking_get_event(event_list, event_id, empty_list);
}

/// Gets the seat with the given index from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event Event to get the seat from.
/// @param index Index of the seat to get.
/// @return Pointer to the seat.
static unsigned int* get_seat_with_delay(struct Event* event, size_t index) {
	struct timespec delay = delay_to_timespec(state_access_delay_ms);
	nanosleep(&delay, NULL);  // Should not be removed

	return &event->data[index];
}

/// Gets the index of a seat.
/// @note This function assumes that the seat exists.
/// @param event Event to get the seat index from.
/// @param row Row of the seat.
/// @param col Column of the seat.
/// @return Index of the seat.
static size_t seat_index(size_t event_cols, size_t row, size_t col) {
	return (row - 1) * event_cols + col - 1;
}

/// Compares integer a with integer b.
/// @param a one of the integers to be compared.
/// @param b the other integer to compare.
/// @return negative if a < b, 0 if a == b, 1 otherwise.
static int size_t_cmp(const void* a, const void* b) {
	size_t a_cmp = *((size_t*) a);
	size_t b_cmp = *((size_t*) b);
	return -1 * (a_cmp < b_cmp) + 1 * (a_cmp > b_cmp);
}

int ems_init(int err, unsigned int delay_ms) {
	if (event_list != NULL) {
		print_str(err, "EMS state has already been initialized\n");
		return 1;
	}

	event_list = create_list();
	state_access_delay_ms = delay_ms;

	return event_list == NULL;
}

int ems_terminate(int err) {
	if (event_list == NULL) {
		print_str(err, "EMS state must be initialized\n"); 
		return 1;
	}

	free_list(event_list);
	event_list = NULL;
	return 0;
}

int ems_create(int err, unsigned int event_id, size_t num_rows, size_t num_cols) {
	if (event_list == NULL) {
		global_output_lock_acquire();
		print_str(err, "EMS state must be initialized\n"); 
		return 1;
	}

	struct Event* event = event_create(event_id, num_rows, num_cols);

	if (event == NULL) {
		global_output_lock_acquire();
		print_str(err, "Error allocating memory for event\n"); 
		return 1;
	}

	int empty_list = 0;
	if (blocking_get_event_with_delay(event_id, &empty_list) != NULL && (!empty_list)) {
		event_free(event);
		global_output_lock_acquire();
		print_str(err, "Event already exists\n"); 
		return 1;
	}

	if (append_to_list(event_list, event) != 0) {
		event_free(event);
		global_output_lock_acquire();
		print_str(err, "Error appending event to list\n"); 
		return 1;
	}

	return 0;
}

int ems_reserve(int err, unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {
	if (event_list == NULL) {
		global_output_lock_acquire();
		print_str(err, "EMS state must be initialized\n"); 
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id);

	if (event == NULL) {
		global_output_lock_acquire();
		print_str(err, "Event not found\n");
		return 1;
	}

	unsigned int reservation_id = event_increment_reservation(event, 1);
	size_t event_cols = event_get_columns(event);
	size_t event_rows = event_get_rows(event);

	size_t idxs[MAX_RESERVATION_SIZE];
	for (size_t i = 0; i < num_seats; i++) { // convert everything to idxs
		size_t row = xs[i];
		size_t col = ys[i];
		if (row <= 0 || row > event_rows || col <= 0 || col > event_cols) {
			event_increment_reservation(event, -1);
			global_output_lock_acquire();
			print_str(err, "Invalid seat\n");
			return 1;
		}
		idxs[i] = seat_index(event_cols, row, col);
	}
	qsort(idxs, num_seats, sizeof(size_t), size_t_cmp); // order idxs

	size_t i = 0;
	for (; i < num_seats; i++) {
		size_t seat = idxs[i];
		if ((i != 0) && (idxs[i] == idxs[i - 1]))
			continue; // skip duplicates
		handle_lock_failure(pthread_rwlock_wrlock(&event->data_locks[seat]));
		if (*get_seat_with_delay(event, seat) != 0) {
			handle_unlock_failure(pthread_rwlock_unlock(&event->data_locks[seat]));
			break;
		}

		*get_seat_with_delay(event, seat) = reservation_id;
	}
	
	int failed = i < num_seats;
	for (size_t j = 0; j < i; j++) { // unlock all locks
		size_t seat = idxs[j];
		if ((j != 0) && (idxs[j] == idxs[j - 1]))
			continue; // skip duplicates
		if (failed)
			*get_seat_with_delay(event, seat) = 0;
		handle_unlock_failure(pthread_rwlock_unlock(&event->data_locks[seat]));
	}

	if (failed) {
		event_increment_reservation(event, -1);
		global_output_lock_acquire();
		print_str(err, "Seat already reserved\n");
		return 1;
	}
	return 0;
}

int ems_show(int out, int err, unsigned int event_id) {
	if (event_list == NULL) {
		global_output_lock_acquire();
		print_str(err, "EMS state must be initialized\n");
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id);

	if (event == NULL) {
		global_output_lock_acquire();
		print_str(err, "Event not found\n");
		return 1;
	}

	size_t event_rows = event_get_rows(event);
	size_t event_cols = event_get_columns(event);
	size_t last_seat = seat_index(event_cols, event_rows, event_cols);
	String* show = string_create(128);
	if(show == NULL) {
		global_output_lock_acquire();
		print_str(err, "Failed to allocate SHOW output\n");
		return 1;
	}

	size_t i = 0;
	for(; i <= last_seat; i++) {
		handle_lock_failure(pthread_rwlock_rdlock(&event->data_locks[i]));
		char seat_buf[MAX_UINT_SIZE + 1];
		show = string_append(show, uint_to_str(event->data[i], seat_buf));
		if(((i + 1) % event_cols) != 0)
			show = string_append(show, " ");
		else
			show = string_append(show, "\n");
		if(show == NULL) {
			handle_unlock_failure(pthread_rwlock_unlock(&event->data_locks[i]));
			break;
		}
	}
	
	for(size_t j = 0; j < i; j++) {
		handle_unlock_failure(pthread_rwlock_unlock(&event->data_locks[j]));
	}

	if(i < last_seat) {
		global_output_lock_acquire();
		print_str(err, "Failed to accumulate SHOW output\n");
		return 1;
	}

	global_output_lock_acquire();
	print_str(out, string_view(show));
	global_output_lock_release();
	string_free(show);
	return 0;
}

int ems_list_events(int out, int err) {
	if (event_list == NULL) {
		global_output_lock_acquire();
		print_str(err, "EMS state must be initialized\n");
		return 1;
	}

	handle_lock_failure(pthread_rwlock_rdlock(&event_list->head_lock));
	struct ListNode* current = event_list->head;
	handle_unlock_failure(pthread_rwlock_unlock(&event_list->head_lock));
	if (current == NULL) {
		global_output_lock_acquire();
		print_str(out, "No events\n");
		return 1;
	}

	String* accumulate = string_create(128);
	if (accumulate == NULL) {
		global_output_lock_acquire();
		print_str(err, "Failed to allocate LIST output\n");
		return 1;
	}

	while (current != NULL) {
		unsigned int id = event_get_id(current->event);
		char id_buf[MAX_UINT_SIZE + 1];
		accumulate = string_append(accumulate, "Event: ");
		accumulate = string_append(accumulate, uint_to_str(id, id_buf));
		accumulate = string_append(accumulate, "\n");
		if (accumulate == NULL) {
			global_output_lock_acquire();
			print_str(err, "Failed to accumulate LIST output\n");
			return 1;
		}
		struct ListNode* old = current;
		handle_lock_failure(pthread_rwlock_rdlock(&old->node_lock));
		current = current->next;
		handle_unlock_failure(pthread_rwlock_unlock(&old->node_lock));
	}

	global_output_lock_acquire();
	print_str(out, string_view(accumulate));
	global_output_lock_release();
	string_free(accumulate);
	return 0;
}

void ems_wait(unsigned int delay_ms) {
	struct timespec delay = delay_to_timespec(delay_ms);
	nanosleep(&delay, NULL);
}
