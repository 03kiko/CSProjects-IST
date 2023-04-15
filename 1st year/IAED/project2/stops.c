/* iaed-23 - ist1106583 - project2 */

/*
 * File: stops.c
 * Author:  Francisco Mónica
 * Description: The file contains all the code related to the stops part of 
 * the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "utility.h"

/* Initializes the head and tail of the stops linked list. */
StopList initStopList() {
    StopList list;
    
    list.head = (list.tail = NULL);

    return list;
}

/* Inserts the stop at the end of the linked list. */
void insertStopEndList(StopList* stopList, Stop* stop) {
    if(stopList->head == NULL)
        stopList->head = (stopList->tail = stop);
    else {
        stopList->tail->next = stop; 
        stopList->tail = stopList->tail->next;
    }
}

/* Removes a stop item from the linked list. */
void removeStopItemList(StopList* list, Stop* stop) {
    Stop *x, *prev;

    for(x = list->head, prev = NULL; x != NULL; prev = x, x = x->next) {
        if(x == stop) {
            if(x == list->head)
                list->head = x->next;
            else
                prev->next = x->next;

            if(x == list->tail)
                list->tail = prev;

            freeStopNode(x);
            break;
        }
    }
}

/* Frees the stops linked list. */
void freeStopList(StopList* stopList) {
    Stop* x = stopList->head;

    while (x != NULL) {
        Stop* next = x->next;
        freeStopNode(x);
        x = next;        
    }

    stopList->tail = NULL;
    stopList->head = NULL;
}

/* Frees a stop node. */
void freeStopNode(Stop* stop) {
    free(stop->name);
    free(stop);
}

/* Generates the hash value based on a string "key", this function is used to
 * get the hash values for the stops hash table. */
int hashStopFunc(char* key, int hashSize) {
    int h = 0;

    for(; *key != '\0'; key++)
        h += *key % hashSize;

    return h % hashSize;
}

/* Initializes the hashtable for the stop items. */
Stop** initStopHash(int hashSize) {
    Stop** links = (Stop**) malloc(sizeof(Stop*)*hashSize);

    if(links == NULL)
        outOfMemory();

    clearStopHash(links, hashSize);

    return links;
}

/* Sets all pointers of the hash table to NULL clearing the hash table. */
void clearStopHash(Stop** stopLinks, int hashSize) {
    int i;

    for(i = 0; i < hashSize; i++)
        stopLinks[i] =NULL;
}

/* Find the stop with the given name and returns a pointer to it, if it doesn't
 * exit returs NULL.
*/
Stop* findStop(char* name, Stop** stopLinks) {
    int i = hashStopFunc(name, HASH_STOP_SIZE);
    Stop* x;

    for(x = stopLinks[i]; x != NULL; x = x->nextH) {
        if(strcmp(x->name, name) == 0)
            return x;
    }

    return NULL;
} 

/* Check if the stop exists. */
int stopExists(char* nameBuffer, Stop** stopLinks) {
    return (findStop(nameBuffer, stopLinks) != NULL);
}

/* Inserts the stop into the hash table in the beginning of the chaining linked
 * list.
*/
void insertBeginStopHash(Stop* stop, Stop** links) {
    int i = hashStopFunc(stop->name, HASH_STOP_SIZE);
    
    stop->nextH = links[i];
    links[i] = stop;
}

/* Removes a stop item from the hash table. */
void removeStopItemHash(Stop** stopLinks, Stop* stop) {
    int i = hashStopFunc(stop->name, HASH_STOP_SIZE);
    Stop *x, *prev;

    for(x = stopLinks[i], prev = NULL; x != NULL; prev = x, x = x->nextH) {
        if(x == stop) {
            if(x == stopLinks[i])
                stopLinks[i] = x->nextH;
            else
                prev->nextH = x->nextH;

            /* Stop item is only freed after it is removed from the linked 
             * list.
            */

            break;
        }
    }
}

/* Main function of the "p" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void stopsCommand(char* input, StopList* stopList, Stop** stopLinks) {
    double lat, lon;
    char nameBuffer[MAX_INPUT_SIZE];

    /* Command line has 3 arguments -> Create new stop. */
    if (sscanf(input, "p \"%[^\"]\" %lf %lf", nameBuffer, &lat, &lon) == 3 ||
        sscanf(input, "p %s %lf %lf", nameBuffer, &lat, &lon) == 3)
        /* \"%[^\"]\" reads the stop name if it's delimited by double quotes. */

        stopCreation(nameBuffer , lat, lon, stopList, stopLinks);
    
    /* Command line has 1 argument -> output stop location. */
    else if (sscanf(input, "p \"%[^\"]\"", nameBuffer) == 1 ||
             sscanf(input, "p %s", nameBuffer) == 1)
                   
        printStopLocation(nameBuffer, stopLinks);
    
    /* command line is "p" -> output all stops. */
    else
        printAllStops(stopList);
}

/* Creates a new stop if it doesn't already exist a stop with the same name, 
 * otherwise outputs a error message. 
*/
void stopCreation(char* nameBuffer, double lat, double lon,
                  StopList* stopList, Stop** stopLinks) {
    
    if(stopExists(nameBuffer, stopLinks))
        printf("%s: stop already exists.\n", nameBuffer);
    else
        createNewStop(nameBuffer, stopList, stopLinks, lat, lon);
}

/* Create new stop and inserts it into the hash table and linked list. */
void createNewStop(char* nameBuffer, StopList* stopList, Stop** stopLinks,
                   double lat, double lon) {
    /* Create new stop. */
    Stop* stop = newStop(nameBuffer, lat, lon);

    /* Insert new stop into the linked list and hash table. */
    insertStopEndList(stopList, stop);
    insertBeginStopHash(stop, stopLinks);
}

/* Creates a new stop, it allocates the memory for the new stop item and
 * assigns the parameters values to the corresponding members of the stop.
*/
Stop* newStop(char* nameBuffer, double lat, double lon) {
    Stop* stop = (Stop*) malloc(sizeof(Stop));
    if(stop == NULL)
        outOfMemory();
        
    stop->name = (char*) malloc(sizeof(char)*(strlen(nameBuffer) + 1));
    if(stop->name == NULL)
        outOfMemory();

    /* Insert parameters and initialize pointers. */
    strcpy(stop->name, nameBuffer);
    stop->latitude = lat;
    stop->longitude = lon;
    stop->numRoutes = 0;
    stop->next = NULL;
    stop->nextH = NULL;

    return stop;
}

/* Prints a specific stop location if it exists, otherwise outputs an error
 * message.
*/
void printStopLocation(char* nameBuffer, Stop** stopLinks) {

    if(stopExists(nameBuffer, stopLinks)) {
        Stop* stop = findStop(nameBuffer, stopLinks);
        printLocation(stop);
    } else
        printf("%s: no such stop.\n", nameBuffer);
}

/* Outputs the location of a given stop. */
void printLocation(Stop* stop) {
    printf("%16.12f %16.12f\n", stop->latitude, stop->longitude);
}

/* Outputs all stops in the system along with their information. */
void printAllStops(StopList* stopList) {
    Stop* x;
    
    for(x = stopList->head; x != NULL; x = x->next)
        printStopInfo(x);
}

/* Outputs a specific stop along with its information. */
void printStopInfo(Stop* x) {
    printf("%s: %16.12f %16.12f %d\n", x->name, x->latitude, x->longitude,
            x->numRoutes);
}

/* Finds the routes that go through a specific stop item and adds them to the 
 * linked list.
*/
StopRoutesNode* findRoutesOfStop(Stop* stop, RouteList* routeList, 
                                 StopRoutesNode* head) {
    Route* x;

    /* Iterate over the routes linked list. */
    for(x = routeList->head; x != NULL; x = x->next) {
        RouteStopNode* y;
        
        /* Iterate over the stops of a route and look for the stop. */
        for(y = x->stops->head; y != NULL; y = y->next) {
            /* Stop Found -> create new node with it and add it to the linked
             * list.
            */

            if(stop == y->stop) {
                StopRoutesNode* node = newStopRoutesNode(x);
                node->next = head;
                head = node;
                break;
            }
        }
    }
    
    return head;
}

/* Creates a new stop routes node, it allocates the memory for the node and 
 * inserts the parameter into the corresponding node member of the node.
*/
StopRoutesNode* newStopRoutesNode(Route* route) {
    StopRoutesNode* node = malloc(sizeof(StopRoutesNode));
    
    if(node == NULL)
        outOfMemory();
    
    /* Insert parameter and initialize pointer. */
    node->next = NULL;
    node->route = route;

    return node;
}

/* Frees all the memory allocated in the system by the linkes list of stop 
 * routes nodes.
*/
void freeStopRoutesNodes(StopRoutesNode* head) {
    StopRoutesNode* x = head;

    while(x != NULL) {
        StopRoutesNode* next = x->next;
        free(x);
        x = next;
    }
}

/* Frees the allocated memory by all the stops in the system and resets the
 * hash table and linked list.
*/
void freeStops(StopList* stopList, Stop** stopLinks) { 
    freeStopList(stopList);
    clearStopHash(stopLinks, HASH_STOP_SIZE);
}

/* Main function of "e" command, which receives the input and calls
 * auxiliary functions to remove a stop from the program if it exists, 
 * otherwise outputs an error message.
*/
void removeStopCommand(char* input, StopList* stopList, Stop** stopLinks,
                       RouteList* routeList, ConnList* connList) {

    char nameBuffer[MAX_INPUT_SIZE];

    if(sscanf(input, "e \"%[^\"]\"", nameBuffer) == 1 ||
       sscanf(input, "e %s", nameBuffer) == 1) {
        if(!stopExists(nameBuffer, stopLinks))
            printf("%s: no such stop.\n", nameBuffer);
        else {
            Stop* stop = findStop(nameBuffer, stopLinks);
            StopRoutesNode* head = NULL;
            
            head = findRoutesOfStop(stop, routeList, head);
            
            head = updateRouteStops(head, connList, stop);

            freeStopRoutesNodes(head);
            
            removeStopItemHash(stopLinks, stop);
            removeStopItemList(stopList, stop);
        }
    }
}

/* Updates the routes that goe through the stop that is going to be deleted. */
StopRoutesNode* updateRouteStops(StopRoutesNode* head, ConnList* connList, 
                                 Stop* stop) {
    StopRoutesNode* x = head;

    /* Iterate the routes linked list. */
    while(x != NULL) {
        RouteStopNode *y = x->route->stops->head, *prev = NULL;

        /* Iterate the stops of the route and search for the stop that is being 
         * deleted.
        */
        while(y != NULL) {
            RouteStopNode* next = y->next;
            
            if(y->stop == stop) {           
                if(deleteStopOfRoute(x, y, stop, connList, next, prev))
                    break;
                
                x->route->numStops--;
                freeRouteStopNode(x->route, y);
                
                /* Search for more ocurrences */
                y = x->route->stops->head;
                prev = NULL;
            } else {
                prev = y;
                y = next;
            }
        }

        /* If the route was left with only one stop reset the route. */
        if(x->route->numStops == 1)
            resetRouteWithOneStop(x);

        x = x->next;
    }

    return head;
}

/* Resets a route that was only left with a single stop. */
void resetRouteWithOneStop(StopRoutesNode* node) {
    node->route->numStops = 0;
    freeRouteStopNode(node->route, node->route->stops->head);
    node->route->stops->head = (node->route->stops->tail = NULL);
}

/* Deletes the stop that is going to be removed of the system from the route. */
int deleteStopOfRoute(StopRoutesNode* stopRoutesNode, RouteStopNode* node,
                      Stop* stop, ConnList* connList, RouteStopNode* next,
                      RouteStopNode* prev) {
    int stopcheck = checkStopConditions(stopRoutesNode, stop);
    
    /* If stop conditions are met return 1. */
    if(stopcheck)
        return 1;
                
    if(node == stopRoutesNode->route->stops->head) {
        stopRoutesNode->route->stops->head = next;
        removeConnAtRightOfStop(node, connList, stopRoutesNode->route, 1);
    } else {
        prev->next = next;
                    
        if(node == stopRoutesNode->route->stops->tail) {
            stopRoutesNode->route->stops->tail = prev;
            removeConnAtLeftOfStop(node, connList, stopRoutesNode->route, 1);
        } else {
            next->prev = prev;
            removeBothConnsOfStop(node, connList, stopRoutesNode->route);
        }
    }

    return 0; 
}

/* Checks if the conditions to stop the inner loop in the "updateRouteStops" 
 * function are met.
*/
int checkStopConditions(StopRoutesNode* node, Stop* stop) {
    /* The route was left with only one stop so return 1 to exit the cicle. */
    if(node->route->numStops == 1)
        return 1;
    /* Route has only to 2 stops and these are both the one to eliminate
     * In this case, reset the route completly and return 1.
    */
    if(node->route->numStops == stopInRoute(node->route, stop, 1) &&
       node->route->numStops == 2) {
        
        node->route->numStops = 0;
        node->route->cost = (node->route->duration = 0);
        freeRouteStopNode(node->route, node->route->stops->head);
        node->route->stops->head = node->route->stops->tail;
        freeRouteStopNode(node->route, node->route->stops->head);
        node->route->stops->head = (node->route->stops->tail = NULL);
        
        return 1;            
    }

    return 0;
}

/* Removes the connection item that connects the stop to the next one in the
 * route.
*/
void removeConnAtRightOfStop(RouteStopNode* node, ConnList* connList,
                             Route* route, int changeCostDuration) {
    Conn *x, *prev;
    
    /* Only update the cost if the stop that is being deleted is the head or 
     * the tail of the route.
    */
    if(changeCostDuration) {
        double cost = node->nextConn->cost;
        double duration = node->nextConn->duration;
        updateCostAndDuration(route, -1*cost, -1*duration);
    }

    for(x = connList->head, prev = NULL; x != NULL; prev = x, x = x->next) {
        if(x == node->nextConn) {
            if(x == connList->head)
                connList->head = x->next;
            else
                prev->next = x->next;

            if(x == connList->tail)
                connList->tail = prev;
           
            node->nextConn = NULL;
            node->next->prevConn = NULL;
            freeConnNode(x);
            break;
        }
    }
}


/* Remove the connection item that connects the stop to the previous one in the
 * route.
*/
void removeConnAtLeftOfStop(RouteStopNode* node, ConnList* connList,
                             Route* route, int changeCostDuration) {
    Conn *x, *prev;

    /* Only update the cost if the stop that is being deleted is the head or 
     * the tail of the route.
    */
    if(changeCostDuration) {
        double cost = node->prevConn->cost;
        double duration = node->prevConn->duration;
        updateCostAndDuration(route, -1*cost, -1* duration);
    }

    for(x = connList->head, prev = NULL; x != NULL; prev = x, x = x->next) {
        if(x == node->prevConn) {
            if(x == connList->head)
                connList->head = x->next;
            else
                prev->next = x->next;

            if(connList->tail == x)
                connList->tail = prev;

            node->prevConn = NULL;
            node->prev->nextConn = NULL;
            freeConnNode(x);
            break;
        }
    }
}

/* Removes the connections that connect the stop to the next and the previous
 * stop in the route. It also creates a new connection between the previous and
 * the next stop.
*/
void removeBothConnsOfStop(RouteStopNode* node, ConnList* connList,
                           Route* route) {
    Conn* conn;
    double cost = node->prevConn->cost + node->nextConn->cost;
    double duration = node->prevConn->duration + node->nextConn->duration;
    Stop* start = node->prevConn->start;
    Stop* end = node->nextConn->end;

    conn = createNewConn(connList, route, start, end, cost, duration);

    /* Remove both connections but don't change the total cost and duration. */
    removeConnAtLeftOfStop(node, connList, route, 0);

    removeConnAtRightOfStop(node, connList, route, 0);

    node->prev->nextConn = conn;
    node->next->prevConn = conn;
}

