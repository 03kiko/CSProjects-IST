#ifndef EMS_OPERATIONS_H
#define EMS_OPERATIONS_H

#include <stddef.h>

/// IMPORTANT: besides ems_init, ems_terminate and ems_wait
/// whenever a function fails it will return 1 AND leave the global
/// OUTPUT LOCK LOCKED in case the caller wants to elaborate the
/// error further.

/// Initializes the EMS state.
/// @param err file descriptor of error output
/// @param delay_ms State access delay in milliseconds.
/// @return 0 if the EMS state was initialized successfully, 1 otherwise.
int ems_init(int err, unsigned int delay_ms);

/// Destroys the EMS state.
/// @param err file descriptor of error output
/// @return 0 if the EMS state was terminated succesfully, 1 otherwise.
int ems_terminate(int err);

/// Creates a new event with the given id and dimensions.
/// @note in case of failure locks the GLOBAL OUTPUT LOCK.
/// @param err file descriptor of error output
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(int err, unsigned int event_id, size_t num_rows, size_t num_cols);

/// Creates a new reservation for the given event.
/// @note in case of failure locks the GLOBAL OUTPUT LOCK.
/// @param err file descriptor of error output
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(int err, unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys);

/// Prints the given event.
/// @note in case of failure locks the GLOBAL OUTPUT LOCK.
/// @param out output file descriptor
/// @param err file descriptor of error output
/// @param event_id Id of the event to print.
/// @return 0 if the event was printed successfully, 1 otherwise.
int ems_show(int out, int err, unsigned int event_id);

/// Prints all the events.
/// @note in case of failure locks the GLOBAL OUTPUT LOCK.
/// @param out output file descriptor
/// @param err file descriptor of error output
/// @return 0 if the events were printed successfully, 1 otherwise.
int ems_list_events(int out, int err);

/// Waits for a given amount of time.
/// @param delay_us Delay in milliseconds.
void ems_wait(unsigned int delay_ms);

#endif  // EMS_OPERATIONS_H
