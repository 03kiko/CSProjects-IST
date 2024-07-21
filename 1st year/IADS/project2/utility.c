/* iaed-23 - istXXXXXXX - project2 */

/*
 * File: utility.c
 * Author:  Francisco Mónica
 * Description: This file contains the implementation of fuctions that are 
 * used everywhere in the code.
*/

#include <stdlib.h>
#include <stdio.h>

#include "utility.h"

/* Frees all the data in the system and resets the hash tables and linked lists
 * of the stops, routes and connections.
*/
void freeSystem(StopList* stopList, Stop** stopLinks, Route** routeLinks,
                RouteList* routeList, ConnList* connList) {
    freeConns(connList);
    freeRoutes(routeList, routeLinks);
    freeStops(stopList, stopLinks);
}

/* Prints error message when the program runs out of memory and ends the
 * program.
*/
void outOfMemory() {
    printf("No memory.\n");
    exit(EXIT_FAILURE);
}