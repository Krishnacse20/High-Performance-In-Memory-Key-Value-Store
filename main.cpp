#include "server.h"

int main() {
    Server server(8080); // Start server at port 8080
    server.run();
    return 0;
}
