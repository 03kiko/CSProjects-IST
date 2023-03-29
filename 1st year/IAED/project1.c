/* iaed-22/23 - project1 */

/*
 * File:  project1.c
 * Author:  Francisco Mónica
 * Description: This file contains all the code related to project1:
 *     -> Constants and global variables for the project(structures, ...);
 *     -> Functions prototypes;
 *     ->  Functions:
 *           - Main (contains a loop to keep reading command lines and calling
 *             the functions of each command depending on the input);
 *           - Stop functions (contains all functions related to command 'p');
 *           - Route functions (contains all functions related to command 'c');
 *           - Connection functions (contains all functions related to command
 *             'l');
 *           - Interchange functions (contains all functions related to command
 *             'i');
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* ---- CONSTANTS AND GLOBAL VARIABLES ---- */

/* The "+ 1" present in the constant values is only for visual purposes
 * (to highlight that was added an extra space for the '\0' char).
*/

/* Maximum sizes for the input and the keyword word "inverso". */
#define MAX_INPUT_SIZE (BUFSIZ + 1)
#define MAX_INVERSO_KEYWORD_SIZE (7 + 1)

/* Maximum numbers of stops and routes. */
#define MAX_STOPS 10000
#define MAX_ROUTES 200

/* Maximum sizes for stop and route names. */
#define MAX_STOP_NAME (50 + 1)
#define MAX_ROUTE_NAME (20 + 1)

/* Constant used to keep the loop in the main function running. */
#define LOOP_RUNNING 1

/* Structure that represents a stop. */
typedef struct {
    char name[MAX_STOP_NAME];
    double latitude;
    double longitude;
    int num_routes; /* Number of routes that the stop belongs to. */
} Stop;

/* Array that contains all stops of the system. */
Stop stops[MAX_STOPS];

/* Number of stops in the system. */
int num_stops;

/* Structure that represents a route. */
typedef struct {
    char name[MAX_ROUTE_NAME];
    double total_cost;
    double total_duration;
    /* Array of stop indexes that belong to the route. */    
    int route_stops[MAX_STOPS]; 
    /* Number of stops in the route. */
    int route_num_stops;
} Route;

/* Array that contains all routes of the system. */
Route routes[MAX_ROUTES];

/* Number of routes in the system. */
int num_routes;

/* --------------------------------------- */

/* -------- FUNCTIONS PROTOTYPES -------- */

/* Stops Command */

void stops_command(char input[]);

int get_stop_index(char stop_name[]);
int stop_exists(char stop_name[]);

void stop_creation(char stop_name[], double lat, double lon);
void create_new_stop(char stop_name[], double latitude, double longitude);

void print_stop_location(char stop_name[]);
void print_location(int index);

void print_all_stops();
void print_stop_info(Stop stop);

/******************/

/* Routes Command */

void routes_command(char input[]);

int get_route_num_stops(char route_name[]);
int is_stop_in_route(int stop_index, char route_name[], int route_index);
void get_route_origin(char route_name[], char route_origin[]);
void get_route_destination(char route_name[], char route_destination[]);
int get_route_index(char route_name[]);
int route_exists(char route_name[]);

void create_route_or_print_stops(char route_name[], char inverso_keyword[]);

int is_keyword_valid(char inverso_keyword[]);
void print_route_stops(char route_name[], int is_reverse);
void print_route_stops_reverse(int route_index, int num_stops_route);
void print_route_stops_normal(int route_index, int num_stops_route);

void create_new_route(char route_name[]);

void print_all_routes();
void print_route_info(Route route);

/*****************/

/* Connections Command */

void connections_command(char input[]);

int invalid_input_checker(char route_name[], char conn_origin[],
                          char conn_destination[], double cost,
                          double duration);
int connection_valid(char route_name[], char conn_origin[],
                     char conn_desination[]);

void update_data(char route_name[], char conn_origin[], char conn_destination[],
                 double cost, double duration);
void add_stop(int route_index, char route_name[], int destination_index,
              int origin_index, int num_stops_in_route);

void add_stop_end(int route_index, int num_stops_in_route,
                  int destination_index);

void shift_indexes_position(int route_index ,int num_stop_in_route);
void add_stop_beginning(int route_index, int origin_index);

void add_initial_stops_to_route(int route_index,int origin_index,
                                int destination_index);

void update_route_cost_and_duration(int route_index, double cost,
                                    double duration);

/**********************/

/* Interchange Points Command */

void interchange_points_command();

void print_routes_of_stop(int stop_index, int num_routes_of_stop);

void find_routes_of_stop(int stop_index, int route_indexes[]);

void sort_routes_alphabetically(int route_indexes[], int left, int right);
void merge(int route_indexes[], int left, int middle, int right);
int compare_route_names(int route_index1, int route_index2);

void print_routes(int num_routes_of_stop, int route_indexes[]);

/*****************************/

/* --------------------------------------- */

/* In the main function, the user's input is read and then the appropriate
 * command function is called.
*/
int main() {
    char input[MAX_INPUT_SIZE];
    
    while (LOOP_RUNNING) {
        
        if(fgets(input, MAX_INPUT_SIZE, stdin) != NULL) {  
            /* Call the appropriate command function based on the first
             * character of the input (command).
            */
            switch (input[0]) {
                case 'q':
                    return EXIT_SUCCESS;
                    break;
                case 'c':
                    routes_command(input);
                    break;
                case 'p':
                    stops_command(input);
                    break;
                case 'l':
                    connections_command(input);
                    break;
                case 'i':
                    interchange_points_command();
                    break;        
            }
        }
    }

    return 0;
}


/* ------ STOPS COMMAND FUNCTIONS ------ */

/* Main function of the "p" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void stops_command(char input[]) {
    double lat = 0, lon = 0;
    char stop_name[MAX_STOP_NAME];

    /* Command line has 3 arguments -> Create new stop. */
    if (sscanf(input, "p \"%[^\"]\" %lf %lf", stop_name, &lat, &lon) == 3 ||
        sscanf(input, "p %s %lf %lf", stop_name, &lat, &lon) == 3)
        /* \"%[^\"]\" reads the stop name if it's delimited by double quotes. */

        stop_creation(stop_name, lat, lon);
    
    /* Command line has 1 argument -> output stop location. */
    else if (sscanf(input, "p \"%[^\"]\"", stop_name) == 1 ||
             sscanf(input, "p %s", stop_name) == 1)
                   
        print_stop_location(stop_name);
    
    /* command line is "p" -> output all stops. */
    else
        print_all_stops();
}

/* Receives a stop name and returns its index in the stops array, if it doesn't
 * exist returns the number of stops in the system.
*/
int get_stop_index(char stop_name[]) {
    int i;

    for (i = 0; i < num_stops; i++) {
        if (strcmp(stop_name, stops[i].name) == 0)
            return i;
    }
    
    return num_stops;
}

/* Receives a stop name and checks if it exists, using the previous function. */
int stop_exists(char stop_name[]) {
    return get_stop_index(stop_name) != num_stops;
}

/* Checks if the name of the stop that is being created already exists, if yes
 * outputs an error message, otherwise calls an auxiliary function to create a
 * new stop.
*/
void stop_creation(char stop_name[], double lat, double lon) {
    if (stop_exists(stop_name)) {
        printf("%s: stop already exists.\n", stop_name);
    } else {
        create_new_stop(stop_name, lat, lon);
        num_stops++;
    }
}

/* Creates a new stop with the received arguments. */
void create_new_stop(char stop_name[], double latitude, double longitude) {
    
    /* The variable "num_stops" not only has the number of existing stops but
     * also is the first empty index in the array "stops". (This also happens
     * with the "num_routes" variable and "routes" array.)
    */
    
    strcpy(stops[num_stops].name, stop_name);
    stops[num_stops].latitude = latitude;
    stops[num_stops].longitude = longitude;

    /* The rest of the struct fields are automatically initialized since it's a
     * global structure.
    */
}

/* Checks if the given stop name exists, if yes calls an auxiliary function to
 * print its location otherwise outputs an error message.
*/
void print_stop_location(char stop_name[]) {
    int stop_index = get_stop_index(stop_name);

    if (stop_exists(stop_name))
        print_location(stop_index);
    else
        printf("%s: no such stop.\n", stop_name);
}

/* Receives a stop index and outputs its location. */
void print_location(int index) {
    printf("%16.12f %16.12f\n", stops[index].latitude, stops[index].longitude);
}

/* Iterates over the "stops" array and calls an auxiliary function to print each
 * stop along with its information.
*/
void print_all_stops() {
    int i;

    for (i = 0; i < num_stops; i++)
        print_stop_info(stops[i]);
}

/* Outputs a specific stop along with its information. */
void print_stop_info(Stop stop) {
    printf("%s: %16.12f %16.12f %d\n", stop.name, stop.latitude,
            stop.longitude, stop.num_routes);
}
/*---------------------------------------*/


/* ----- ROUTES COMMAND FUNCTIONS ----- */

/* Main function of the "c" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void routes_command(char input[]) {
    char route_name[MAX_ROUTE_NAME];
    char inverso_keyword[MAX_INVERSO_KEYWORD_SIZE];

    /* Command line has 2 arguments -> trying to create a new route or print
     * one's stops in reverse order. */
    if (sscanf(input, "c %s %s",route_name, inverso_keyword) == 2)
        create_route_or_print_stops(route_name, inverso_keyword);
    
    /* Command line has 1 argument -> trying to print route stops in normal
     * order or trying to create a new one.
    */
    else if (sscanf(input, "c %s",route_name) == 1)
        create_route_or_print_stops(route_name, "\0");
    
    /* Command line is "c" -> output all routes. */
    else
        print_all_routes();
}

/* Returns the number of stops in a given route, if the route doesn't exist
 * returns -1.
*/
int get_route_num_stops(char route_name[]) {
    int i;
    
    if(route_exists(route_name)) {
        for (i = 0; i < num_routes; i++) {
            if (strcmp(routes[i].name, route_name) == 0)
                return routes[i].route_num_stops;
        }
        /* Route has 0 stops. */
        return 0;
    }

    /* Route doesn't exist. */
    return -1;
}

/* Checks if a given stop belongs to a given route, if the route doesn't exist
 * the for loop won't run and the function will return 0, also before running
 * the loop it's checked if the stop exists or not.
*/
int is_stop_in_route(int stop_index, char route_name[], int route_index) {
    int i;
    /* Get the number of stops in the route with the given name. */
    int num_stops_in_route = get_route_num_stops(route_name);
    
    if (0 > stop_index || stop_index >= num_stops)
        return 0; /* Stop doesn't exist. */

    for (i = 0; i < num_stops_in_route; i++) {   
        if (routes[route_index].route_stops[i] == stop_index)
            return 1;
    }

    return 0;
}

/* Copies the origin of a given route and copies it to the "route_origin"
 * variable. The function also checks if the route exists or not.
*/
void get_route_origin(char route_name[], char route_origin[]) {
    int i;

    if (!route_exists(route_name))
        return;

    for (i = 0; i < num_routes; i++) {
        if (strcmp(route_name, routes[i].name) == 0) {
            int index = routes[i].route_stops[0];
            
            strcpy(route_origin, stops[index].name);
            break;
        }    
    }
}

/* Copies the destination of a given route and copies it to the
 * "route_destination" variable. The function also checks if the route exists.
*/
void get_route_destination(char route_name[], char route_destination[]) {
    int i;
    
    if (!route_exists(route_name))
        return;

    for (i = 0; i < num_routes; i++)
        if (strcmp(route_name, routes[i].name) == 0) {
            int num_stops_in_route = routes[i].route_num_stops;
            int index = routes[i].route_stops[num_stops_in_route - 1];
            
            strcpy(route_destination, stops[index].name);
            break;
        }
}

/* Receives a route name and returns its index in the "routes" array, if it 
 * doesn't exist returns the number of existent routes.
*/
int get_route_index(char route_name[]) {
    int i;

    for (i = 0; i < num_routes; i++) {
        if (strcmp(route_name, routes[i].name) == 0)
            return i;
    }

    return num_routes;
}

/* Receives a route name and checks if it exists, using the previous function.*/
int route_exists(char route_name[]) {
    return get_route_index(route_name) != num_routes;
}

/* Checks if the route exists, if yes calls an auxiliary function to print the
 * stops of the route (normal or reverse order) otherwise creates a new one
 * with the given name.
*/
void create_route_or_print_stops(char route_name[], char inverso_keyword[]) {
    if (route_exists(route_name)) {
        if (strcmp(inverso_keyword, "\0") != 0)  {
            if(is_keyword_valid(inverso_keyword))
                print_route_stops(route_name, 1);
            else
                printf("incorrect sort option.\n");
        } else
            print_route_stops(route_name, 0);
    } else
        create_new_route(route_name);    
}

/* Checks if the keyword "inverso" was  abbreviated correctly */
int is_keyword_valid(char inverso_keyword[]) {
    return (strcmp(inverso_keyword, "inverso") == 0 ||
            strcmp(inverso_keyword, "invers") == 0 ||
            strcmp(inverso_keyword, "inver") == 0 ||
            strcmp(inverso_keyword, "inve") == 0 ||
            strcmp(inverso_keyword, "inv") == 0);
}

/* This function uses auxiliary functions to calculate the index and the
 * number of stops in the given route, and then calls another function to print
 * the stops of the given route in reverse or normal order based on the
 * value of "is_reverse" variable" argument.
*/
void print_route_stops(char route_name[], int is_reverse) {
    int route_index = get_route_index(route_name);
    int num_stops_in_route = get_route_num_stops(route_name);

    if (is_reverse)
        print_route_stops_reverse(route_index, num_stops_in_route);
    else
        print_route_stops_normal(route_index, num_stops_in_route);
}

/* Outputs the stops of a given route in reverse order. */
void print_route_stops_reverse(int route_index, int num_stops_in_route) {
    int i;

    for (i = num_stops_in_route - 1; i >= 0; i--) {
        int stop_index = routes[route_index].route_stops[i];
        
        if (i == 0)
            printf("%s\n", stops[stop_index].name);
        else
            printf("%s, ", stops[stop_index].name);    
    }
}

/* Outputs the stops of a given route in normal order. */
void print_route_stops_normal(int route_index, int num_stops_in_route) {
    int i;

    for (i = 0; i < num_stops_in_route ; i++) {
        int stop_index = routes[route_index].route_stops[i];
        
        if (i + 1 == num_stops_in_route)
            printf("%s\n", stops[stop_index].name);
        else
            printf("%s, ", stops[stop_index].name);
    }
}

/* Creates a new route with the name received. */
void create_new_route(char route_name[]) {
    /* All other structure fields are automatically initialized. */
    
    strcpy(routes[num_routes].name, route_name);
    num_routes++;
}

/* Iterates over the routes and calls another function to print the specific
 * route information.
*/
void print_all_routes() {
    int i;

    for (i = 0; i < num_routes; i++)
        print_route_info(routes[i]);
}

/* Outputs a specific route along with its information. */
void print_route_info(Route route) {
    /* Checks if the route has stops or not. */
    if (get_route_num_stops(route.name) == 0)
        printf("%s %d %.2f %.2f\n", route.name, route.route_num_stops,
                route.total_cost, route.total_duration);
    else {
        char origin[MAX_ROUTE_NAME], destination[MAX_ROUTE_NAME];
        
        get_route_origin(route.name, origin);
        get_route_destination(route.name, destination);
        
        printf("%s %s %s %d %.2f %.2f\n", route.name, origin,
                destination, route.route_num_stops, route.total_cost,
                route.total_duration);
    }
}
/*---------------------------------------*/

/* --- CONNECTIONS COMMAND FUNCTIONS --- */

/* Main function of the "l" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void connections_command(char input[]) {
    char route_name[MAX_ROUTE_NAME];
    char conn_origin[MAX_STOP_NAME], conn_destination[MAX_STOP_NAME];
    double cost, duration;
    
    /* This if statement reads all the possibilities types of input (names of
     * origin and destination with/without double quotes).
    */
    if (sscanf(input, "l %s \"%[^\"]\" \"%[^\"]\" %lf %lf", route_name,
               conn_origin, conn_destination, &cost, &duration) == 5 ||
        sscanf(input, "l %s %s \"%[^\"]\" %lf %lf", route_name, conn_origin,
               conn_destination, &cost, &duration) == 5 ||
        sscanf(input, "l %s \"%[^\"]\" %s %lf %lf", route_name, conn_origin,
               conn_destination, &cost, &duration) == 5 ||
        sscanf(input, "l %s %s %s %lf %lf ", route_name, conn_origin,
               conn_destination, &cost, &duration) == 5) {
        
        
        if (!invalid_input_checker(route_name, conn_origin, conn_destination,
                                   cost, duration))
            /* Input valid -> Update the data in the system. */
            update_data(route_name, conn_origin, conn_destination, cost,
                        duration);
    }
}

/* Checks if the given input is valid to establish a new connection, returning
 * 1 if it's invalid and 0 otherwise.
*/
int invalid_input_checker(char route_name[], char conn_origin[],
                          char conn_destination[], double cost,
                          double duration) {
    
    int error_printed = 0;
    
    if (!route_exists(route_name)) {
        printf("%s: no such line.\n", route_name);
        error_printed = 1;
    } else if (!stop_exists(conn_origin)) {
        printf("%s: no such stop.\n", conn_origin);
        error_printed = 1;
    } else if (!stop_exists(conn_destination)) {    
        printf("%s: no such stop.\n", conn_destination);
        error_printed = 1;
    } else if (!connection_valid(route_name, conn_origin, conn_destination)) {
        printf("link cannot be associated with bus line.\n");
        error_printed = 1;
    } else if (cost < 0 || duration < 0) {
        printf("negative cost or duration.\n");
        error_printed = 1;
    }

    return error_printed;
}

/* Checks if the connection is valid by checking if the new stop is being
 * added either at the end or at the beginning of the route or if the route
 * has no stops.
*/
int connection_valid(char route_name[], char conn_origin[],
                     char conn_desination[]) {
    
    char route_origin[MAX_STOP_NAME], route_destination[MAX_STOP_NAME];

    get_route_origin(route_name, route_origin);
    get_route_destination(route_name, route_destination);

    
    return (get_route_num_stops(route_name) == 0 ||
            strcmp(conn_origin,route_destination) == 0 ||
            strcmp(conn_desination, route_origin) == 0);
}

/* Updates the data of the system by calling auxiliary functions. */
void update_data(char route_name[], char conn_origin[], char conn_destination[],
                 double cost, double duration) {
    
    int route_index = get_route_index(route_name);
    int origin_index = get_stop_index(conn_origin);
    int destination_index = get_stop_index(conn_destination);
    int num_stops_in_route = get_route_num_stops(route_name);

    /* Route has stops */
    if (num_stops_in_route)
        /* Add a stop (at the end or at the begining). */
        add_stop(route_index, route_name, destination_index, origin_index,
                 num_stops_in_route);
    /* Route has no stops  */
    else
        /* Update origin and destination of the route. */
        add_initial_stops_to_route(route_index, origin_index, 
                                   destination_index); 
    
    /* Update cost and duration */
    update_route_cost_and_duration(route_index, cost, duration);
}

/* Adds a new stop to the route, either at the begining or at the end. */
void add_stop(int route_index, char route_name[], int destination_index,
              int origin_index, int num_stops_in_route) {
    
    int end_index = routes[route_index].route_num_stops - 1;

    /* Adds new stop at the end of the route. */
    if (routes[route_index].route_stops[end_index] == origin_index) {
        /* Check if the stop was already part of the route */
        if (!is_stop_in_route(destination_index, route_name, route_index))
            stops[destination_index].num_routes++;
            
        add_stop_end(route_index, num_stops_in_route, destination_index);      
    } else {
        /* Adds new stop at the beginning of the route. */
               
        /* Shift the indexes of the array one position to the right. */
        shift_indexes_position(route_index, num_stops_in_route);

        if (!is_stop_in_route(origin_index, route_name, route_index))
            stops[origin_index].num_routes++;

        /* Insert new stop. */
        add_stop_beginning(route_index, origin_index);            
    }
}

/* Adds new stop at the end of the route. */
void add_stop_end(int route_index, int num_stops_in_route,
                  int destination_index) {
    
    routes[route_index].route_stops[num_stops_in_route] = destination_index;
    routes[route_index].route_num_stops++;
}

/* Shifts the position of the stop indexes 1 position to the right. */
void shift_indexes_position(int route_index ,int num_stop_in_route) {
    int i;

    for (i = num_stop_in_route; i > 0; i--) {
        int temp = routes[route_index].route_stops[i-1];
        routes[route_index].route_stops[i]= temp;
    }
}

/* Adds new stop at the beginning of the route. */
void add_stop_beginning(int route_index, int origin_index) {
    routes[route_index].route_stops[0] = origin_index;
    routes[route_index].route_num_stops++; 
}

/* Adds initial origin and destination stops to a route. */
void add_initial_stops_to_route(int route_index,int origin_index,
                                int destination_index) {

    routes[route_index].route_stops[0] = origin_index;
    routes[route_index].route_stops[1] = destination_index;
    
    routes[route_index].route_num_stops += 2;

    /* Update the number of routes that each stop belongs to. */
    stops[origin_index].num_routes++;
    stops[destination_index].num_routes++;
}

/* Updates the cost and duration of a specific route. */
void update_route_cost_and_duration (int route_index, double cost,
                                     double duration) {
    routes[route_index].total_cost += cost;
    routes[route_index].total_duration += duration;
}
/*--------------------------------------*/

/* INTERCHANGE POINTS COMMAND FUNCTIONS */

/* Main function of the "i" command which iterates through the stops array and
 * calls auxiliary functions to print the stops that belong to more than one 
 * route along with the route names that they belong to sorted alphabetically
*/
void interchange_points_command() {
    int i;
    
    for (i = 0; i < num_stops; i++) {
        if (stops[i].num_routes > 1) {
            int num_routes_of_stop = stops[i].num_routes;

            printf("%s %d: ", stops[i].name, num_routes_of_stop);
            print_routes_of_stop(i, num_routes_of_stop);
        }
    } 
}

/* Prints all the routes that the stop belongs to in alphabeticall order by
 * calling auxiliary functions. */
void print_routes_of_stop(int stop_index, int num_routes_of_stop) {
    int route_indexes[MAX_ROUTES];
    
    /* Search for all the route indexes which the stop belongs to and adds
     * them to the "route_indexes" array.
    */
    find_routes_of_stop(stop_index, route_indexes);

    /* Sort the routes. (merge sort) */
    sort_routes_alphabetically(route_indexes, 0, num_routes_of_stop - 1);
    
    /* Print routes. */
    print_routes(num_routes_of_stop, route_indexes);
}

/* Finds all the route indexes that the stop at the given index belongs to. */
void find_routes_of_stop(int stop_index, int route_indexes[]) {
    int i,j;
    int num_routes_found = 0;

    /* Iterating by the "routes" array and in each route looking for the given
     * stop index in the "route_stops" array.
    */
    for (i = 0; i < num_routes; i++) {
        /* Used to only add a route index once even if the stop appears
         * multiples times in the route.
        */
        int stop_found = 0; 
        int route_num_stops = get_route_num_stops(routes[i].name);

        for (j = 0; j < route_num_stops; j++) {
        /* The stop is in this route -> add it to the "route_indexes" array. */
            if (routes[i].route_stops[j] == stop_index && !stop_found) {
                
                route_indexes[num_routes_found] = i;
                num_routes_found++;
                stop_found = 1;
    
                if (num_routes_found == route_num_stops)
                    break;    
            }
        }  
    }
}

/* Sort the routes of the given array of route indexes in alphabetical order.*/
void sort_routes_alphabetically(int route_indexes[], int left, int right) {
    int middle = (right + left) / 2;

    if(right <= left)
        return;
    
    /* Recursively sort the left and right halves of the array.*/
    sort_routes_alphabetically(route_indexes, left, middle);
    sort_routes_alphabetically(route_indexes, middle + 1, right);
    merge(route_indexes, left, middle, right);
}

/* Merges two sorted halves of the given array of route indexes. */
void merge(int route_indexes[], int left, int middle, int right) {
    int i, j, k;
    int aux[MAX_ROUTES];

    /* Building auxiliary vector */
    
    /* Left part */
    for (i = middle + 1; i > left; i--)
        aux[i-1] = route_indexes[i-1];
    /* Right part */
    for (j = middle; j < right; j++) {
        aux[right + middle - j] = route_indexes[j+1];
    }

    /* Sorting the vector */
    for (k = left; k <= right; k++) {
        if (compare_route_names(aux[j], aux[i]) || i == middle + 1)
            route_indexes[k] = aux[j--];
        else
            route_indexes[k] = aux[i++];
    }    
}

/* Compares two route names, if route name 1 comes first by alphabetical order
 * returns true, otherwise returns false.
*/
int compare_route_names(int route_index1, int route_index2) {
    return (strcmp(routes[route_index1].name, routes[route_index2].name) < 0);
}

/* Prints the route names of the array of route indexes. */
void print_routes(int num_routes_of_stop, int route_indexes[]) {
    int i;
    
    for (i = 0; i < num_routes_of_stop; i++) {
        int index = route_indexes[i];
        
        if (i + 1 == num_routes_of_stop)        
            printf("%s\n", routes[index].name);
        else
            printf("%s ", routes[index].name);
    }
}
/*-------------------------------------*/