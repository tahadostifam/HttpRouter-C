#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "http_router.h"

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    size_t _ = read(client_socket, buffer, BUFFER_SIZE - 1);

    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    printf("Request: %s %s %s\n", method, path, protocol);

    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello from my C server!</h1></body></html>";
    _ = write(client_socket, response, strlen(response));

    close(client_socket);
}

void add_route(Router *router, const char method, const char path, http_handler handler) {
    if (router->route_count < MAX_ROUTES) {
        Route route;
        strcpy
        route.method = method;
        // strcpy(route.method, method);
        // strcpy(route.path, path);
        // route.handler = handler;
        // router->routes[router->route_count++] = route;
    }
}

Router* new_http_router() {
    Router *router = malloc(sizeof(Router));
    
    if ((router->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(router->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &router->opt, sizeof(router->opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    router->address.sin_family = AF_INET;
    router->address.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(router->server_fd, (struct sockaddr *)&router->address, sizeof(router->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(router->server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return router;
}

void run_http_router(Router *router, int port) {
    int addrlen = sizeof(router->address);
    router->address.sin_port = htons(port);
    printf("Server listening on port %d\n", port);
    
    while (1) {
        if ((router->new_socket = accept(router->server_fd, (struct sockaddr *)&router->address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        handle_client(router->new_socket);
    }
}