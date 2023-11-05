/* iaed-23 - istXXXXXXX - project2 */

/*
 * File: input.c
 * Author:  Francisco Mónica
 * Description: The file contains the implementation of the input functions.
*/

#include "input.h"

/* Calls the appropriate command functions based on the first
 * character of the input (command) by calling auxiliary functions.
*/
void handleInput(char* input, StopList* stopList, RouteList* routeList,
                 ConnList* connList, Stop** stopLinks, Route** routeLinks,
                 int* exitFlag) {
    handleInputP1(input, stopList, routeList, connList, stopLinks, routeLinks,
                  exitFlag);
    handleInputP2(input, stopList, routeList, connList, stopLinks, routeLinks);
}

/* Calls the appropriate command functions based on the first character of the 
 * input (command). (in this functions are the commands of the first project). 
*/
void handleInputP1(char* input, StopList* stopList, RouteList* routeList,
                   ConnList* connList, Stop** stopLinks, Route** routeLinks,
                   int* exitFlag) {
    switch(input[0]) {
        case 'q':
            *exitFlag = 1;
            break;
        case 'c':
            routesCommand(input, routeList, routeLinks);
            break;
        case 'p':
            stopsCommand(input, stopList, stopLinks);
            break;
        case 'l':
            connectionsCommand(input, connList, routeLinks, stopLinks);
            break;
        case 'i':
            interchangePointsCommnand(stopList, routeList);
            break;
    }
}

/* Calls the appropriate command functions based on the first character of the 
 * input (command). (in this function are the commands of second project).
*/
void handleInputP2(char* input, StopList* stopList, RouteList* routeList,
                  ConnList* connList, Stop** stopLinks, Route** routeLinks) {
    switch(input[0]) {
        case 'r':
            removeRouteCommand(input, routeList, routeLinks, connList);
            break;
        case 'e':
            removeStopCommand(input, stopList, stopLinks, routeList, connList);
            break;
        case 'a':
            freeSystem(stopList, stopLinks, routeLinks, routeList, connList);
            break; 
    }
}

