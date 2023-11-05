/* iaed-23 - istXXXXXXX - project2 */

/*
 * File: project2.c
 * Author:  Francisco Mónica
 * Description: Main file of the project, which has the main function in which 
 * is a loop to keep the program running and calling the correct functions.
*/

#include <stdlib.h>
#include <stdio.h>

#include "data.h"
#include "input.h"

/* In the main function, the user's input is read and then the appropriate
 * command function is called, when quitting the program the memory is freed.
*/
int main() {
    int exitFlag = 0;
    char input[MAX_INPUT_SIZE];

    StopList stopList;
    RouteList routeList;
    ConnList connList;

    Stop** stopLinks;
    Route** routeLinks;

    stopList = initStopList();
    routeList = initRouteList();
    connList = initConnList();
    
    stopLinks = initStopHash(HASH_STOP_SIZE);
    routeLinks = initRouteHash(HASH_ROUTE_SIZE);

    while (!exitFlag)
        if(fgets(input, MAX_INPUT_SIZE, stdin) != NULL)  
           handleInput(input, &stopList, &routeList, &connList, stopLinks, 
                       routeLinks, &exitFlag);

    freeSystem(&stopList, stopLinks, routeLinks, &routeList, &connList);

    /* Free system completly. */
    free(stopLinks);
    free(routeLinks);

    return 0;
}