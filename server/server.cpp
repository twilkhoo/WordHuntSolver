#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "solver.h"
#include "util.h"

int main(int argc, char** argv) {
  if (argc > 2)
    throw std::invalid_argument("Usage: ./server OR ./server <serverPortNum>");

  unsigned int port = 5000;  //  Default TCP server port.
  if (argc == 2) port = std::stoi(argv[1]);

  ServerUtil serverObj = ServerUtil(port);

  // Create TCP socket for server.
  if (!serverObj.openTcp())
    throw std::runtime_error("Unable to create TCP socket.");

  // Bind to port.
  if (!serverObj.bindToPort())
    throw std::runtime_error("Unable to bind port to TCP socket.");

  // Listen for incoming connections.
  if (!serverObj.listenConn())
    throw std::runtime_error("Error in listening for clients.");

  std::cout << "Server listening on port " << port << "\n";

  while (1) {
    // Accept a client connection.
    auto [connSuccess, clientFd] = serverObj.acceptConn();
    if (!connSuccess) continue;

    std::cout << "Accepted a client.\n";

    std::thread clientThread([&serverObj, clientFd]() {
      if (!serverObj.handleClient(clientFd)) {
        std::cout << "Error in processing this client.\n";
      }
    });
    clientThread.join();
  }

  // Close the server socket
  serverObj.closeServer();
  return 0;
}
