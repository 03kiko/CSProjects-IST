/* iaed-23 - ist1106583 - project2 */

/*
 * File: connections.c
 * Author:  Francisco Mónica
 * Description: This file contains the implementation of fuctions related to 
 * the connections part of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "utility.h"

/* Initializes the head and tail of the connections linked list. */
ConnList initConnList() {
    ConnList list;

    list.head = (list.tail = NULL);

    return list;
}

/* Inserts the new connection at the end of the linked list. */
void insertConnEndList(Conn* conn, ConnList* connList) {
    if(connList->head == NULL) {
        connList->head = (connList->tail = conn);
    } else {
        connList->tail->next = conn;
        connList->tail = connList->tail->next;
    }
}

/* Main function of the "l" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void connectionsCommand(char* input, ConnList* connList, Route** routeLinks,
                        Stop** stopLinks) {
    
    char routeBuffer[MAX_INPUT_SIZE];
    char startBuffer[MAX_INPUT_SIZE];
    char endBuffer[MAX_INPUT_SIZE];
    double cost, duration;

    /* This if statement reads all the possibilities types of input (names of
     * origin and destination with/without double quotes).
    */
    if (sscanf(input, "l %s \"%[^\"]\" \"%[^\"]\" %lf %lf", routeBuffer,
               startBuffer, endBuffer, &cost, &duration) == 5 ||
        sscanf(input, "l %s %s \"%[^\"]\" %lf %lf", routeBuffer, startBuffer,
               endBuffer, &cost, &duration) == 5 ||
        sscanf(input, "l %s \"%[^\"]\" %s %lf %lf", routeBuffer, startBuffer,
               endBuffer, &cost, &duration) == 5 ||
        sscanf(input, "l %s %s %s %lf %lf ", routeBuffer, startBuffer,
               endBuffer, &cost, &duration) == 5) {
        
        if (validInput(routeBuffer, startBuffer, endBuffer, cost, duration,
                        routeLinks, stopLinks)) {
            /* valid input -> Update the data in the system & create new 
             * connection.
            */

            Route* route = findRoute(routeBuffer, routeLinks);
            Stop* start = findStop(startBuffer, stopLinks);
            Stop* end = findStop(endBuffer, stopLinks);
            Conn* conn;
            
            conn = createNewConn(connList, route, start, end, cost, duration);
            
            /* Update data. */
            updateData(conn);
        }
    }
}

/* Checks if the given input is valid to establish a new connection, returning
 * 1 if it's valid and 0 otherwise.
*/
int validInput(char* routeBuffer, char* startBuffer, char* endBuffer,
               double cost, double duration, Route** routeLinks,
               Stop** stopLinks) {
    
    if(!routeExists(routeBuffer, routeLinks)) {
        printf("%s: no such line.\n", routeBuffer);
        return 0;
    } else if(!stopExists(startBuffer, stopLinks)) {
        printf("%s: no such stop.\n", startBuffer);
        return 0;
    } else if(!stopExists(endBuffer, stopLinks)) {
        printf("%s: no such stop.\n", endBuffer);
        return 0;
    } else if(!connectionValid(routeBuffer, startBuffer, endBuffer,
                               routeLinks)) {
        printf("link cannot be associated with bus line.\n");
        return 0;
    } else if(cost < 0 || duration  < 0) {
        printf("negative cost or duration.\n");
        return 0;
    }
    
    return 1;
}

/* Checks if the connection is valid by checking if the new stop is being
 * added either at the end or at the beginning of the route or if the route
 * has no stops.
*/
int connectionValid(char* routeBuffer, char* startBuffer, char* endBuffer,
                    Route** routeLinks) {
    
    Route* route = findRoute(routeBuffer, routeLinks);
    
    return(route->numStops == 0 ||
           strcmp(route->stops->head->stop->name, endBuffer) == 0 ||
           strcmp(route->stops->tail->stop->name, startBuffer) == 0);
}

/* Creates a new conn and inserts it into the linked list.*/
Conn* createNewConn(ConnList* connList, Route* route, Stop* start, Stop* end,
                    double cost, double duration) {
    /* Create new connection. */
    Conn* conn = newConn(start, end ,route, cost, duration);
            
    /* Insert new connection. */
    insertConnEndList(conn, connList);

    return conn;
}

/* Creates a new connection. It allocates the memory for a new connection and 
 * assigns the input parameters values to the corresponding member variables of
 * the connection item.
*/
Conn* newConn(Stop* start, Stop* end, Route* route, double cost,
              double duration) {
    Conn* conn = (Conn*) malloc(sizeof(Conn));

    if(conn == NULL)
       outOfMemory();
    
    /* Insert parameters. */
    conn->cost = cost;
    conn->duration = duration;
    conn->end = end;
    conn->next = NULL;
    conn->route = route;
    conn->start = start;

    return conn;
}

/* Updates the system data by inserting the new connection data in the system
 * (add stop(s) to the route, increase numbers of stops and routes, add cost 
 * and duration).
*/
void updateData(Conn* conn) {
    addStops(conn, conn->route, conn->start, conn->end);

    updateCostAndDuration(conn->route, conn->cost, conn->duration);
}

/* Adds the stop(s) to the specific route. */
void addStops(Conn* conn, Route* route, Stop* start, Stop* end) {
    if(route->stops->head == NULL)
        /* empty route. */
        addInitialStopsToRoute(conn, route,start, end);
    else if(start == route->stops->tail->stop)
        /* adding stop at the end. */
        addStopToRouteEnd(conn, route, end);
    else
        /* adding stop at the beginning. */
        addStopToRouteBeginning(conn, route, start);
}

/* Adds initial stops to a route. */
void addInitialStopsToRoute(Conn* conn, Route* route, Stop* start, Stop* end) {
    /* Create 2 new nodes. */
    RouteStopNode* startNode = newRouteStopNode(start);
    RouteStopNode* endNode = newRouteStopNode(end);

    /* Insert both nodes and update numbers of stops and routes. */
    route->numStops += 2;
    
    route->stops->head = startNode;
    start->numRoutes++;
    
    route->stops->tail = endNode;

    if(!stopInRoute(route, end, 0))
        end->numRoutes++;
    
    /* Connect head and tail. */
    route->stops->head->next = route->stops->tail;
    route->stops->tail->prev = route->stops->head;

    route->stops->head->nextConn = conn;
    route->stops->tail->prevConn = conn;
}

/* Adds new stop at the end of the route. */
void addStopToRouteEnd(Conn* conn, Route* route, Stop* end) {
    /* Create new node. */
    RouteStopNode* node = newRouteStopNode(end);
    
    /* Insert node and update numbers of stops and routes. */
    route->numStops++;
    
    if(!stopInRoute(route, end, 0))
        end->numRoutes++;

    route->stops->tail->nextConn = conn;
    route->stops->tail->next = node;
    node->prev = route->stops->tail;
    
    route->stops->tail = node;
    route->stops->tail->prevConn = conn;
}

/* Adds new stop at the beginning of the route. */
void addStopToRouteBeginning(Conn* conn, Route* route, Stop* start) {
    /* Create new node. */
    RouteStopNode* node = newRouteStopNode(start);
    
    /* Insert node and update numbers of stops and routes. */
    route->numStops++;
    
    if(!stopInRoute(route, start, 0))
        start->numRoutes++;
    
    route->stops->head->prevConn = conn;
    node->next = route->stops->head;
    route->stops->head->prev = node;
    
    route->stops->head = node;
    route->stops->head->nextConn = conn;
}

/* Frees the allocated memory by all the connections in the system and resets 
 * the linked list. */
void freeConns(ConnList* connList) {
    Conn* x = connList->head;

    while(x != NULL) {
        Conn* next = x->next;
        freeConnNode(x);
        x = next;
    }

    connList->tail = NULL;
    connList->head = NULL;
}

/* Frees a connection node. */
void freeConnNode(Conn* conn) {
    free(conn);
}