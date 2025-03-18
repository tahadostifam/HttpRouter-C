#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define MAX_ROUTES 100

typedef void (*http_handler)(int socket);

typedef struct Route
{
    char method[16];
    char path[256];
    http_handler handler;
} Route;

typedef struct Router
{
    int opt;
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    Route routes[MAX_ROUTES];
    int route_count;
} Router;

Router* new_http_router();
void run_http_router(Router *router, int port);
void add_route(Router *router, const char *method, const char *path, http_handler handler);

#endif //HTTP_ROUTER_H
