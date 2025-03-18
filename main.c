#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "http_router.h"

void home_action(int socket) {
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Home!</h1></body></html>";
    write(socket, response, strlen(response));

    close(socket);
}

void about_action(int socket) {
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>About!</h1></body></html>";
    write(socket, response, strlen(response));

    close(socket);
}

int main() {
    Router* router = new_http_router();

    add_route(router, "GET", "/", home_action);
    add_route(router, "GET", "/about", about_action);

    run_http_router(router, 8080);
    free_http_router(router);
}   