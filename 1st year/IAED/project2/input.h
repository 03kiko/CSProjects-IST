/* iaed-23 - ist1106583 - project2 */

/*
 * File: input.h
 * Author:  Francisco Mónica
 * Description: The file contains the functions prototypes used to read the 
 * input.
*/

#ifndef _INPUT_H_
#define _INPUT_H_

#include "data.h"
#include "utility.h"

void handleInput(char* input, StopList* stopList, RouteList* routeList,
                 ConnList* connList, Stop** stopLinks, Route** routeLinks,
                 int* exitFlag);
void handleInputP1(char* input, StopList* stopList, RouteList* routeList,
                   ConnList* connList, Stop** stopLinks, Route** routeLinks,
                   int* exitFlag);
void handleInputP2(char* input, StopList* stopList, RouteList* routeList,
                  ConnList* connList, Stop** stopLinks, Route** routeLinks);

#endif