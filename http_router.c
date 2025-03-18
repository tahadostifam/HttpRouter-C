#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "http_router.h"

Router* new_http_router() {
    Router *router = malloc(sizeof(Router));
    int opt = 1;
    
    if ((router->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(router->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    router->address.sin_family = AF_INET;
    router->address.sin_addr.s_addr = INADDR_ANY;

    return router;
}

void lookup_routes(Router *router) {
    char buffer[BUFFER_SIZE] = {0};
    read(router->new_socket, buffer, BUFFER_SIZE - 1);
    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    for (int i = 0; i < router->route_count; i++) {
        if (strcmp(router->routes[i].method, method) == 0 && strcmp(router->routes[i].path, path) == 0) {
            router->routes[i].handler(router->new_socket);
            return;
        }
    }

    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Not Found!</h1></body></html>";
    write(router->new_socket, response, strlen(response));
}

void run_http_router(Router *router, int port) {
    router->address.sin_port = htons(8080);
    
    if (bind(router->server_fd, (struct sockaddr *)&router->address, sizeof(router->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(router->server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(router->address);
    router->address.sin_port = htons(port);
    printf("Server listening on port %d\n", port);
    
    while (1) {
        if ((router->new_socket = accept(router->server_fd, (struct sockaddr *)&router->address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        lookup_routes(router);
    }
}

void free_http_router(Router *router) {
    close(router->server_fd);
    free(router);
}

void add_route(Router *router, const char *method, const char *path, http_handler handler) {
    if (router->route_count < MAX_ROUTES) {
        Route route;
        strcpy(route.method, method);
        strcpy(route.path, path);
        route.handler = handler;
        router->routes[router->route_count++] = route;
    }
}

void write_string(int socket, const char *str) {
    write(socket, str, strlen(str));
}
