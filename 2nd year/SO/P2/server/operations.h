#ifndef SERVER_OPERATIONS_H
#define SERVER_OPERATIONS_H

#include <stddef.h>

/// Initializes the EMS state.
/// @param delay_us Delay in microseconds.
/// @return 0 if the EMS state was initialized successfully, 1 otherwise.
int ems_init(unsigned int delay_us);

/// Destroys the EMS state.
/// @Note not multithreading safe! Before using this function,
/// one must ensure ONLY 1 thread is working on the ems.
/// @return 0 on success, 1 otherwise.
int ems_terminate();

/// Creates a new event with the given id and dimensions.
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols);

/// Creates a new reservation for the given event.
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys);

/// Sends the seats of the requested event to a client.
/// @param out_fd to write to.
/// @param event_id Id of the event to show
/// @return 1 on failure; 0 otherwise; IO_FAILURE if server couldn't read/write
/// a message successfully (because the client disconnected for instance)
int ems_show(int out_fd, unsigned int event_id);

/// Gets the number of existent events along with each ones id
/// @out_fd to write events to.
/// @return 1 on failure; 0 otherwise; IO_FAILURE if server couldn't read/write
/// a message successfully (because the client disconnected for instance)
int ems_list_events(int out_fd);

/// FIXME maybe change these functions
/// @param out_fd 
/// @return 
int ems_display_events(int out_fd);


#endif  // SERVER_OPERATIONS_H
