#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Basic "pong" response with CORS headers
    const char *pong_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: http://localhost:3000\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "Content-Length: 4\r\n"
        "\r\n"
        "pong";

    // Basic "wrong" response with CORS headers
    const char *wrong_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: http://localhost:3000\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "wrong";

    // Preflight (OPTIONS) response
    const char *options_response =
        "HTTP/1.1 204 No Content\r\n"
        "Access-Control-Allow-Origin: http://localhost:3000\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind to port 5000
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        // Clear the buffer and read data from the client
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read < 0) {
            perror("read failed");
            close(client_fd);
            continue;
        }

        // 1) Parse out the HTTP method (e.g., GET, POST, OPTIONS)
        char method[16] = {0};
        sscanf(buffer, "%15s", method);

        // 2) Check if it's a preflight OPTIONS request
        if (strcmp(method, "OPTIONS") == 0) {
            write(client_fd, options_response, strlen(options_response));
            close(client_fd);
            continue;
        }

        // 3) Otherwise, handle normal requests
        // Check if the request contains "ping"
        if (strstr(buffer, "ping") != NULL) {
            write(client_fd, pong_response, strlen(pong_response));
        } else {
            write(client_fd, wrong_response, strlen(wrong_response));
        }

        // Close the client socket
        close(client_fd);
    }

    // Close the server socket
    close(server_fd);
    return 0;
}
