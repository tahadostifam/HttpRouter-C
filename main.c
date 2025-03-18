#include "http_router.h"

int main() {
    Router* router = new_http_router();


    
    run_http_router(router, 8080);
}   