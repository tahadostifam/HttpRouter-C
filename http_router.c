#include "http_router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Router* new_http_router() {
    Router *router = (Router *)malloc(sizeof(Router));
    if (!router) {
        perror("Failed to allocate memory for router");
        return NULL;
    }
    router->route_count = 0;
    return router;
}

void run_http_router(Router *router, int port) {
    printf("Running HTTP router on port %d\n", port);
    // Implementation details...
}

void free_http_router(Router *router) {
    if (router) {
        free(router);
    }
}

void add_route(Router *router, const char *method, const char *path, http_handler handler) {
    if (router->route_count < MAX_ROUTES) {
        strcpy(router->routes[router->route_count].method, method);
        strcpy(router->routes[router->route_count].path, path);
        router->routes[router->route_count].handler = handler;
        router->route_count++;
    }
}

void write_string(int socket, const char *str) {
    send(socket, str, strlen(str), 0);
}
