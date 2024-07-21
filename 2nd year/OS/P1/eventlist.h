#ifndef EVENT_LIST_H
#define EVENT_LIST_H

#include <pthread.h>
#include <stddef.h>

struct Event {
	unsigned int id;            /// Event id
	unsigned int reservations;  /// Number of reservations for the event.

	size_t cols;  /// Number of columns.
	size_t rows;  /// Number of rows.
	
	pthread_mutex_t event_lock; /// Lock for accessing reservations

	unsigned int* data;  /// Array of size rows * cols with the reservations for each seat.
	pthread_rwlock_t* data_locks; // Lock for each seat
};

struct ListNode {
	struct Event* event;
	struct ListNode* next;
	pthread_rwlock_t node_lock;
};

// Linked list structure
struct EventList {
	struct ListNode* head;  // Head of the list
	struct ListNode* tail;  // Tail of the list
	pthread_rwlock_t head_lock; // Protects the entry into the first node
};

/// Creates a new event.
/// @param id of the event.
/// @param rows of the event.
/// @param columns of the event.
/// @return pointer to the event, NULL on failure
struct Event* event_create(unsigned int id, size_t rows, size_t cols);

/// Frees an event.
/// @param event to free.
int event_free(struct Event* event);

/// Atomic access to the event's row size.
/// @param event to get the rows from.
/// @return how many rows the event has.
size_t event_get_rows(const struct Event* event);

/// Atomic access to the event's columns.
/// @param event get the columns from.
/// @return how many colums the event has.
size_t event_get_columns(const struct Event* event);

/// Atomic access to the event's id.
/// @param event to get the id from.
/// @return id of the event.
unsigned int event_get_id(const struct Event* event);

/// Atomically increments (or decrements if inc is negative) the current
/// reservation of the event and returns it.
/// @Note does not guard against underflows or overflows.
/// @param event to increment the reservation of.
/// @return reservation after increment.
unsigned int event_increment_reservation(struct Event* event, int inc);

/// Creates a new event list.
/// @return Newly created event list, NULL on failure
struct EventList* create_list();

/// Appends a new node to the list.
/// @Note expects the last node of the list to be locked beforehand.
/// @param list Event list to be modified.
/// @param data Event to be stored in the new node.
/// @return 0 if the node was appended successfully, 1 otherwise.
int append_to_list(struct EventList* list, struct Event* data);

/// Removes a node from the list.
/// @param list Event list to be modified.
/// @return 0 if the node was removed successfully, 1 otherwise.
int free_list(struct EventList* list);

/// Retrieves an event in the list.
/// @param list Event list to be searched
/// @param event_id Event id.
/// @return Pointer to the event if found, NULL otherwise.
struct Event* get_event(struct EventList* list, unsigned int event_id);

/// Retrieves an event in the list.
/// @Note if it does not find the desired event, then it will leave
/// the mutex of the last node locked for further operations if so desired.
/// If the list is empty, empty_list will be set to 1, and the head_lock
/// will be left locked. If the event is found, no locks will be left locked.
/// @param list to get the event from.
/// @param event_id of the event.
/// @param empty_list flag set to true if the list is empty.
/// @return NULL if the event is not found, or the list is empty.
/// Pointer to the event otherwise.
struct Event* blocking_get_event(struct EventList* list, unsigned int event_id, int* empty_list);

#endif  // EVENT_LIST_H
