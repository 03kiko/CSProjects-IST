/* iaed-23 - ist1106583 - project2 */

/*
 * File: data.h
 * Author:  Francisco Mónica
 * Description: Definition of all structures and functions prototypes of the 
 * program related to data.
*/

#ifndef _DATA_H_
#define _DATA_H_

/* CONSTANTS */
/* Hash Table sizes. */
#define HASH_STOP_SIZE 49999
#define HASH_ROUTE_SIZE 1999

/* Maximum input and "inverso" keyword sizes. */
#define MAX_INPUT_SIZE (65535 + 1)
#define MAX_INVERSO_KEYWORD_SIZE (7 + 1)

/* Structure that represents a stop. */
typedef struct stop {
    char* name;
    double latitude;
    double longitude;
    int numRoutes; /* Number of routes that go through this stop. */
    struct stop* next; /* Next stop in the linked list. */
    struct stop* nextH;
} Stop;

/* Strucutre that contains the head and tail of the stops linked list. */
typedef struct {
    Stop* head; 
    Stop* tail;
} StopList;

/* Node of stops that belong to the route. */
typedef struct routeStopNode {
    Stop* stop;
    struct connection* prevConn;
    struct connection* nextConn;
    struct routeStopNode* next;
    struct routeStopNode* prev;
} RouteStopNode;

/* Structure that contains the head and tail of the stops of a route.*/
typedef struct {
    RouteStopNode* head;
    RouteStopNode* tail;
} RouteStops;

/* Structure that represents a route. */
typedef struct route {
    char* name;
    double cost;
    double duration;
    RouteStops* stops;
    int numStops; /* Number of stops that are in this route. */
    struct route* next; /* Next route in the linked list. */
    struct route* nextH;
} Route;

/* Structure that represents a connection. */
typedef struct connection {
    Route* route;
    Stop* start;
    Stop* end;
    double cost;
    double duration;
    struct connection* next;
} Conn;

/* Structure that contains the end and tail of the connections list. */
typedef struct {
    Conn* head;
    Conn* tail;
} ConnList;

/* Structure that represents a node of a route that goes through a specific
 * stop.
*/
typedef struct stopRoutesNode {
    Route* route;
    struct stopRoutesNode* next;
} StopRoutesNode;

/* Structure that contains the head and tail of the route linked list. */
typedef struct {
    Route* head;
    Route* tail;
} RouteList;

/* FUNCTION PROTOTYPES */

StopList initStopList();
void insertStopEndList(StopList* stopList, Stop* stop);
void removeStopItemList(StopList* list, Stop* stop);
void freeStopList(StopList* stopList);
void freeStopNode(Stop* stop);
int hashStopFunc(char* key, int hashSize);
Stop** initStopHash(int hashSize);
void clearStopHash(Stop** stopLinks, int hashSize);
Stop* findStop(char* name, Stop** stopLinks);
int stopExists(char* nameBuffer, Stop** stopLinks);
void insertBeginStopHash(Stop* stop, Stop** links);
void removeStopItemHash(Stop** stopLinks, Stop* stop);
void stopsCommand(char* input, StopList* stopList, Stop** stopLinks);
void stopCreation(char* nameBuffer, double lat, double lon,
                  StopList* stopList, Stop** stopLinks);
void createNewStop(char* nameBuffer, StopList* stopList, Stop** stopLinks,
                   double lat, double lon);
Stop* newStop(char* nameBuffer, double lat, double lon);
void printStopLocation(char* nameBuffer, Stop** stopLinks);
void printLocation(Stop* stop);
void printAllStops(StopList* stopList);
void printStopInfo(Stop* x);
StopRoutesNode* findRoutesOfStop(Stop* stop, RouteList* routeList, 
                                 StopRoutesNode* head);
StopRoutesNode* newStopRoutesNode(Route* route);
void freeStopRoutesNodes(StopRoutesNode* head);
void freeStops(StopList* stopList, Stop** stopLinks);
void removeStopCommand(char* input, StopList* stopList, Stop** stopLinks,
                       RouteList* routeList, ConnList* connList);
StopRoutesNode* updateRouteStops(StopRoutesNode* head, ConnList* connList, 
                                 Stop* stop);
void resetRouteWithOneStop(StopRoutesNode* node);
int deleteStopOfRoute(StopRoutesNode* stopRoutesNode, RouteStopNode* node,
                      Stop* stop, ConnList* connList, RouteStopNode* next,
                      RouteStopNode* prev);
int checkStopConditions(StopRoutesNode* node, Stop* stop);
void removeConnAtRightOfStop(RouteStopNode* node, ConnList* connList,
                             Route* route, int changeCostDuration);
void removeConnAtLeftOfStop(RouteStopNode* node, ConnList* connList,
                             Route* route, int changeCostDuration);
void removeBothConnsOfStop(RouteStopNode* node, ConnList* connList,
                           Route* route);

RouteList initRouteList();
void insertRouteEndList(RouteList* routeList, Route* route);
void removeRouteItemList(RouteList* list, Route* route);
void freeRouteList(RouteList* routeList);
void freeRouteNode(Route* route);
void freeRouteStopNode(Route* route, RouteStopNode* routeStopNode);
int hashRouteFunc(char* key, int hashSize);
Route** initRouteHash(int hashSize);
void clearRouteHash(Route** routeLinks, int hashSize);
Route* findRoute(char* name, Route** routeLinks);
int routeExists(char* name, Route** routeLinks);
void insertBeginRouteHash(Route* route, Route** routeLinks);
void removeRouteItemHash(Route** routeLinks, Route* route);
void routesCommand(char* input, RouteList* routeList, Route** routeLinks);
void createRouteOrPrintStops(char* nameBuffer, char* inverso,
                             RouteList* routeList, Route** routeLinks);
int inversoValid(char* inverso);
void printRouteStopsReverse(char* nameBuffer, Route** routeLinks);
void printRouteStopsNormal(char* nameBuffer, Route** routeLinks);
void createNewRoute(char* nameBuffer, RouteList* routeList,
                    Route** routeLinks);
Route* newRoute(char* nameBuffer);
RouteStopNode* newRouteStopNode(Stop* stop);
void printAllRoutes(RouteList* routeList);
void printRouteInfo(Route* x);
int stopInRoute(Route* route, Stop* stop, int count);
void updateCostAndDuration(Route* route, double cost, double duration);
void freeRoutes(RouteList* routeList, Route** routeLinks);
void removeRouteCommand(char* input, RouteList* routeList, Route** routeLinks,
                        ConnList* connList);
void removeRoute(Route* route, ConnList* connList, Route** routeLinks, 
                 RouteList* routeList);
void removeConnsByRoute(ConnList* list, Route* route);

ConnList initConnList();
void insertConnEndList(Conn* conn, ConnList* connList);
void connectionsCommand(char* input, ConnList* connList, Route** routeLinks,
                        Stop** stopLinks);
int validInput(char* routeBuffer, char* startBuffer, char* endBuffer,
               double cost, double duration, Route** routeLinks,
               Stop** stopLinks);
int connectionValid(char* routeBuffer, char* startBuffer, char* endBuffer,
                    Route** routeLinks);
Conn* createNewConn(ConnList* connList, Route* route, Stop* start, Stop* end,
                    double cost, double duration);
Conn* newConn(Stop* start, Stop* end, Route* route, double cost,
              double duration);
void updateData(Conn* conn);
void addStops(Conn* conn, Route* route, Stop* start, Stop* end);
void addInitialStopsToRoute(Conn* conn, Route* route, Stop* start, Stop* end);
void addStopToRouteEnd(Conn* conn, Route* route, Stop* end);
void addStopToRouteBeginning(Conn* conn, Route* route, Stop* start);
void freeConns(ConnList* connList);
void freeConnNode(Conn* conn);

void interchangePointsCommnand(StopList* stopList, RouteList* routeList);
void printRoutesOfStop(Stop* stop, RouteList* routeList);
StopRoutesNode* sortListAlphabetically(StopRoutesNode* head);
void swapNodes(StopRoutesNode* a, StopRoutesNode* b);
void printRoutes(StopRoutesNode* head);

/**********************/

#endif
