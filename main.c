#include <stdio.h>
#include <stdlib.h>
#include "http_router.h"

// void home_action(int socket) {
    
// }

int main() {
    Router* router = new_http_router();

    // add_route(router, "GET", "/", home_action);
    // printf("%s\n", router->routes[0].path);

    run_http_router(router, 8080);
    free_http_router(router);
}   