/* iaed-23 - istXXXXXXX - project2 */

/*
 * File: interchangePoints.c
 * Author:  Francisco Mónica
 * Description: The file contains all the code related to the intersection 
 * points part of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "utility.h"

/* Main function of the "i" command which iterates through the stops linked 
 * list and calls auxiliary functions to print the stops that belong to more 
 * than one route along with the route names that they belong to sorted 
 * alphabetically.
*/
void interchangePointsCommnand(StopList* stopList, RouteList* routeList) {
    Stop* x;

    for(x = stopList->head; x != NULL; x = x->next) {
        if(x->numRoutes > 1) {
            printf("%s %d: ", x->name, x->numRoutes);
            printRoutesOfStop(x, routeList);
        }
    }
}

/* Finds all the routes that pass through a stop, sorts them alphabetically 
 * and then frees all the memory allocated used for it.
*/
void printRoutesOfStop(Stop* stop, RouteList* routeList) {
    StopRoutesNode* head = NULL;
    
    /* Create a linked list with all the routes that pass through this stop. */
    head = findRoutesOfStop(stop, routeList, head);

    /* Sort the routes alphabetically (bubble sort for linked lists). */
    head = sortListAlphabetically(head);

    /* Print routes. */
    printRoutes(head);

    /* Free the allocated memory. */
    freeStopRoutesNodes(head);
}

/* Sorts a linked list alphabetically by the routes names (bubble sort). */
StopRoutesNode* sortListAlphabetically(StopRoutesNode* head) {
    int done;
    StopRoutesNode *ptr1, *ptr2 = NULL;

    /* Keeps sorting untill the list is sorted (indicated by the "done" 
     * variable).
    */
    do {
        done = 1;
        ptr1 = head;
        
        /* At every iteration the "ptr1" is set to the head of the list, 
         * "ptr2" variable is used to sort one less item at each iteration.
        */
        while(ptr1->next != ptr2) {
            if(strcmp(ptr1->route->name, ptr1->next->route->name) > 0) {
                swapNodes(ptr1, ptr1->next);
                done = 0;
            }

            ptr1 = ptr1->next;
        }

        ptr2 = ptr1;
    } while(!done);

    return head;
}

/* Swaps the data of 2 nodes. */
void swapNodes(StopRoutesNode* a, StopRoutesNode* b) {
    Route* aux = a->route;
    
    a->route = b->route;
    b->route = aux;
}

/* Prints the list of routes that pass through a stop. */
void printRoutes(StopRoutesNode* head) {
    StopRoutesNode* x;

    for(x = head; x != NULL; x = x->next) {
        if(x->next == NULL)
            printf("%s\n", x->route->name);
        else
            printf("%s ", x->route->name);
    }
}