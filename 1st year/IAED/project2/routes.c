/* iaed-23 - ist1106583 - project2 */

/*
 * File: routes.c
 * Author:  Francisco Mónica
 * Description: The file contains all the code related to the routes part of 
 * the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "utility.h"

/* Initializes the head and tail of the routes linked list. */
RouteList initRouteList() {
    RouteList list;

    list.head = (list.tail = NULL);

    return list;
}

/* Inserts route at the end of the linked list. */
void insertRouteEndList(RouteList* routeList, Route* route) {
    if(routeList->head == NULL)
        routeList->head = (routeList->tail = route);
    else {
        routeList->tail->next = route; 
        routeList->tail = route;
    }
}

/* Removes a route item from the linked list. */
void removeRouteItemList(RouteList* list, Route* route) {
    Route *x, *prev;

    for(x = list->head, prev = NULL; x != NULL; prev = x, x = x->next) {
        if(x == route) {
            if(x == list->head)
                list->head = x->next;
            else
                prev->next = x->next;

            if(x == list->tail)
                list->tail = prev;

            freeRouteNode(x);
            break;
        }
    }
}

/* Frees the routes linked list. */
void freeRouteList(RouteList* routeList) {
    Route* x = routeList->head;

    while(x != NULL) {
        Route* next = x->next;
        freeRouteNode(x);
        x = next;
    }

    routeList->tail = NULL;
    routeList->head = NULL;
}

/* Frees a route Node. */
void freeRouteNode(Route* route) {
    RouteStopNode* y = route->stops->head;
    
    while(y != NULL) {
        RouteStopNode* nextStop = y->next;
        freeRouteStopNode(route, y);
        route->stops->head = nextStop;
        y = nextStop;
    }

    free(route->name);
    free(route->stops);
    free(route);
}

/* Frees a route Stop Node. */
void freeRouteStopNode(Route* route, RouteStopNode* routeStopNode) {
    if(stopInRoute(route, routeStopNode->stop, 1) == 1)
        routeStopNode->stop->numRoutes--;

    free(routeStopNode);
}

/* Generates the hash value based on a string "key", this function is used to
 * get the hash values for the routes hash table. */
int hashRouteFunc(char* key, int hashSize) {
    int h = 0;

    for(; *key != '\0'; key++)
        h += *key % hashSize;

    return h % hashSize;
}

/* Initializes the hashtable for the route items. */
Route** initRouteHash(int hashSize) {
    Route** links = (Route**) malloc(sizeof(Route*)*hashSize);

    if(links == NULL)
        outOfMemory();

    clearRouteHash(links, hashSize);

    return links;
}

/* Reset route hash table. */
void clearRouteHash(Route** routeLinks, int hashSize) {
    int i;

    for (i = 0; i < hashSize; i++)
        routeLinks[i] = NULL;
}

/* Finds the route with the given name and returns a pointer to it, if it
 * doesn't exist returns NULL.
*/
Route* findRoute(char* name, Route** routeLinks) {
    int i = hashRouteFunc(name, HASH_ROUTE_SIZE);
    Route* x;
    
    for(x = routeLinks[i]; x != NULL; x = x->nextH) {
        if(strcmp(x->name, name) == 0)
            return x;
    }

    return NULL;
}

/* Check if the route with the given name exists. */
int routeExists(char* name, Route** routeLinks) {
    return (findRoute(name, routeLinks) != NULL);
}

/* Inserts route into the hash table at the beginning of the chaining linked 
 * list.
*/
void insertBeginRouteHash(Route* route, Route** routeLinks) {
    int i = hashRouteFunc(route->name, HASH_ROUTE_SIZE);

    route->nextH = routeLinks[i];
    routeLinks[i] = route;
}

/* Removes a route item from the hash table. */
void removeRouteItemHash(Route** routeLinks, Route* route) {
    int i = hashRouteFunc(route->name, HASH_ROUTE_SIZE);
    Route *x, *prev;

    for(x = routeLinks[i], prev = NULL; x != NULL; prev = x, x = x->nextH) {
        if(x == route) {
            if(x == routeLinks[i])
                routeLinks[i] = x->nextH;
            else
                prev->nextH = x->nextH;
            /* Route item is only fread after it is removed from the linked 
             * list.
            */
            break;
        }
    }
}

/* Main function of the "c" command, which receives the input and calls other
 * auxiliary functions according to the number of arguments of the input.
*/
void routesCommand(char* input, RouteList* routeList, Route** routeLinks) {
    char nameBuffer[MAX_INPUT_SIZE];
    char inverso[MAX_INVERSO_KEYWORD_SIZE];

    /* Command line has 2 arguments -> Trying to create a new route or print
     * one's stops in reverse order. */
    if (sscanf(input, "c %s %s",nameBuffer, inverso) == 2)
        createRouteOrPrintStops(nameBuffer, inverso, routeList, routeLinks);
    
    /* Command line has 1 argument -> Trying to print route stops in normal
     * order or trying to create a new one.
    */
    else if (sscanf(input, "c %s",nameBuffer) == 1)
        createRouteOrPrintStops(nameBuffer, NULL, routeList, routeLinks);
    
    /* Command line is "c" -> Output all routes. */
    else
        printAllRoutes(routeList);
}

/* Checks if the route exists, if yes calls an auxiliary function to print the
 * stops of the route (normal or reverse order) otherwise creates a new one
 * with the given name.
*/
void createRouteOrPrintStops(char* nameBuffer, char* inverso,
                             RouteList* routeList, Route** routeLinks) {
    if(routeExists(nameBuffer, routeLinks)) {
        if(inverso != NULL) {
            if(inversoValid(inverso))
                printRouteStopsReverse(nameBuffer, routeLinks);
            else
                printf("incorrect sort option.\n");
        } else
            printRouteStopsNormal(nameBuffer, routeLinks);
    } else
        createNewRoute(nameBuffer, routeList, routeLinks);
}

/* Check if the word "inverso" is abbreviated correctly. */
int inversoValid(char* inverso) {
    return (strcmp(inverso, "inverso") == 0 ||
            strcmp(inverso, "invers") == 0 ||
            strcmp(inverso, "inver") == 0 ||
            strcmp(inverso, "inve") == 0 ||
            strcmp(inverso, "inv") == 0);
}

/* Outputs the stops of a given route in reverse order. */
void printRouteStopsReverse(char* nameBuffer, Route** routeLinks) {
    Route* route = findRoute(nameBuffer, routeLinks);
    RouteStopNode* x;

    for(x = route->stops->tail; x != NULL; x = x->prev) {
        if(x->prev == NULL)
            printf("%s\n", x->stop->name);
        else
            printf("%s, ", x->stop->name);
    }
}

/* Outputs the stops of a given route in normal order. */
void printRouteStopsNormal(char* nameBuffer, Route** routeLinks) {
    Route* route = findRoute(nameBuffer, routeLinks);
    RouteStopNode* x;

    for(x = route->stops->head; x != NULL; x = x->next) {        
        if(x->next == NULL)
            printf("%s\n", x->stop->name);
        else
            printf("%s, ", x->stop->name);
    }
}

/* Creates a new route, inserts it in the linked list and hash table. */
void createNewRoute(char* nameBuffer, RouteList* routeList,
                    Route** routeLinks) {
    /* Create new route. */
    Route* route = newRoute(nameBuffer);
        
    /* Insert it in the linked list and in the hash table. */
    insertRouteEndList(routeList, route);
        
    insertBeginRouteHash(route, routeLinks);
}

/* Creates a new route. It allocates the memory for a new route and assigns the
 * input parameter value to the corresponding member variable of the route
 * item.
*/
Route* newRoute(char* nameBuffer) {
    Route* route = (Route*) malloc(sizeof(Route));
    if(route == NULL)
        outOfMemory();
      
    route->name = (char*) malloc(sizeof(char) * (strlen(nameBuffer) + 1));
    if(route->name == NULL)
        outOfMemory();

    route->stops = (RouteStops*) malloc(sizeof(RouteStops));
    if(route->stops == NULL)
        outOfMemory();
    
    /* Insert parameter and initialize other variable members. */
    strcpy(route->name, nameBuffer);
    route->cost = 0;
    route->duration = 0;
    route->next = NULL;
    route->nextH = NULL;
    route->numStops = 0;
    route->stops->head = (route->stops->tail = NULL);

    return route;
}

/* Creates a new Route stop node, it allocates memory for a new routeStopNode 
 * and assigns the input parameter value to the corresponding member.
*/
RouteStopNode* newRouteStopNode(Stop* stop) {
    RouteStopNode* node =(RouteStopNode*) malloc(sizeof(RouteStopNode));

    if(node == NULL)
        outOfMemory();
    
    /* Insert parameter and initialize pointers. */
    node->stop = stop;
    node->nextConn = (node->prevConn = NULL);
    node->prev = (node->next = NULL);
 
    return node;
}

/* Prints all routes along with their information. */
void printAllRoutes(RouteList* routeList) {
    Route* x;

    for(x = routeList->head; x != NULL; x = x->next)
        printRouteInfo(x);
}

/* Outputs a specific route and its information. */
void printRouteInfo(Route* x) {
    if(x->numStops != 0)
        printf("%s %s %s %d %.2f %.2f\n", x->name,
               x->stops->head->stop->name, x->stops->tail->stop->name, 
               x->numStops, x->cost, x->duration);
    else
        printf("%s %d %.2f %.2f\n", x->name, x->numStops, x->cost, 
                x->duration);
}

/* Checks if a given stop belongs to a given route and optionally counts the 
 * number of times it appears in it.
*/
int stopInRoute(Route* route, Stop* stop, int count) {
    RouteStopNode* x;
    int stopCount = 0;

    for(x = route->stops->head; x != NULL; x = x->next) {
        if(x->stop == stop) {
            if(count)
                stopCount++;
            else
                return 1;
        }
    }

    return (stopCount);
}

/* Updates the cost and duration of a specific route. */
void updateCostAndDuration(Route* route, double cost, double duration) {
    route->cost += cost;
    route->duration += duration;
}

/* Frees the allocated memory by the all routes in the system and resets the
 * hash table and linked list.
*/
void freeRoutes(RouteList* routeList, Route** routeLinks) {
    freeRouteList(routeList);
    clearRouteHash(routeLinks, HASH_ROUTE_SIZE);
}

/* Main function of the "r" command, which receives the input and calls 
 * auxiliary functions to remove a route from the program if it exists, 
 * otherwise outputs and error message.
*/
void removeRouteCommand(char* input, RouteList* routeList, Route** routeLinks,
                        ConnList* connList) {
    char nameBuffer[MAX_INPUT_SIZE];

    if(sscanf(input, "r \"%[^\"]\"", nameBuffer) == 1 ||
       sscanf(input, "r %s", nameBuffer) == 1) {
        
        if(!routeExists(nameBuffer, routeLinks))
            printf("%s: no such line.\n", nameBuffer);
        else {
            Route* route = findRoute(nameBuffer, routeLinks);
            
            removeRoute(route, connList, routeLinks, routeList);
        }
    }    
}

/* Removes a given route from the system. */
void removeRoute(Route* route, ConnList* connList, Route** routeLinks, 
                 RouteList* routeList) {
    removeConnsByRoute(connList, route);
    removeRouteItemHash(routeLinks, route);
    removeRouteItemList(routeList, route);
}

/* Removes all connections related to a specific route. */
void removeConnsByRoute(ConnList* list, Route* route) {
    Conn *x = list->head, *prev = NULL;

    while(x != NULL) {
        Conn* next = x->next;
        
        if(x->route == route) {
            if(x == list->head)
                list->head = x->next;
            else
                prev->next = x->next;

            if(x == list->tail)
                list->tail = prev;

            freeConnNode(x);
        } else
            prev = x;
        x = next;
    }
}