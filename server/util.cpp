#include "util.h"

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "solver.h"

ServerUtil::ServerUtil(unsigned int port_)
    : port{port_}, trie("../assets/words_alpha.txt") {}

bool ServerUtil::openTcp() {
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd < 0) return false;

  int opt = 1;  //  1 to enable the option.
  int setOptResult =
      setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (setOptResult < 0) {
    close(serverFd);
    return false;
  }

  this->serverFd = serverFd;
  return true;
}

bool ServerUtil::bindToPort() {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    return false;
  return true;
}

bool ServerUtil::listenConn() {
  if (listen(serverFd, 3) < 0) return false;
  return true;
}

bool ServerUtil::acceptConn() {
  socklen_t addrLen = sizeof(address);

  clientFd = accept(serverFd, (struct sockaddr *)&address, &addrLen);
  if (clientFd < 0) return false;

  return true;
}

bool ServerUtil::handleClient() {
  // Clear the buffer and read data from the client
  memset(buffer, 0, BUFFER_SIZE);

  std::cout << "Reading\n";

  // Read the HTTP request from the client.
  ssize_t bytes_read = read(clientFd, buffer, BUFFER_SIZE - 1);
  if (bytes_read <= 0) {
    close(clientFd);
    return false;
  }
  std::cout << "Done read.\n";

  std::string bufferStr(buffer, bytes_read);

  std::cout << "Message received: " << bufferStr << "\n";

  // Parse the first line (the GET request).
  std::size_t endOfLine = bufferStr.find("\r\n");
  if (endOfLine == std::string::npos) {
    // If we can't find "\r\n", the request is malformed
    std::cout << "Malformed request (no first line found).\n";
    return false;
  }
  std::string firstLine = bufferStr.substr(0, endOfLine);
  std::cout << "First line: " << firstLine << "\n";

  // Split the first line into request, path, HTTP version tokens.
  std::istringstream lineStream(firstLine);

  std::string method;
  std::string path;
  std::string version;

  if (!(lineStream >> method >> path >> version)) {
    std::cout << "Malformed request line.\n";
    return false;
  }

  if (method != "GET") {
    std::cout << "Unsupported HTTP method: " << method << "\n";
    return 1;
  }

  std::cout << "path we got is: " << path << "\n";

  // Form the board based on the comma separated string.
  if (!path.empty() && path[0] == '/') {
    path = path.substr(1);  // remove the first character '/'
  }

  // 2) Split by commas into tokens
  std::vector<std::string> tokens;
  {
    std::stringstream ss(path);
    std::string piece;
    while (std::getline(ss, piece, ',')) {
      // e.g. piece could be "a", "b", ..., or "kl"
      tokens.push_back(piece);
    }
  }

  // 3) Flatten tokens into individual chars.
  //    For example, "kl" becomes 'k' and 'l'
  std::vector<char> allChars;
  for (const auto &tok : tokens) {
    for (char c : tok) {
      allChars.push_back(c);
    }
  }

  // We expect exactly 16 chars (4x4). Check (optional).
  if (allChars.size() != 16) {
    std::cerr << "Error: we got " << allChars.size()
              << " characters, expected 16.\n";
    return 1;
  }

  // 4) Put them in a 4x4 2D vector
  std::vector<std::vector<char>> grid(4, std::vector<char>(4));
  {
    int index = 0;
    for (int row = 0; row < 4; ++row) {
      for (int col = 0; col < 4; ++col) {
        grid[row][col] = allChars[index++];
      }
    }
  }

  // 5) Print the 4x4 grid
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      std::cout << grid[row][col] << ' ';
    }
    std::cout << '\n';
  }

  // Solve the board using the solver.
  Solver::WordFinder s(grid, trie);

  std::cout << s.getFoundWords().size() << "\n";

  write(clientFd, invalid_response, strlen(invalid_response));

  std::cout << "Closing client socket.\n";
  // Close the client socket
  close(clientFd);

  return true;
}

void ServerUtil::closeServer() { close(serverFd); }
