/* iaed-23 - istXXXXXXX - project2 */

/*
 * File: utility.h
 * Author:  Francisco Mónica
 * Description: This file contains the function prototypes of fuctions that are 
 * used everywhere in the code.
*/

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "data.h"

void freeSystem(StopList* stopList, Stop** stopLinks, Route** routeLinks,
                RouteList* routeList, ConnList* connList);
void outOfMemory();

#endif