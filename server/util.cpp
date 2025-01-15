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

  if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0)
    return false;
  return true;
}

bool ServerUtil::listenConn() {
  if (listen(serverFd, 3) < 0) return false;
  return true;
}

bool ServerUtil::acceptConn() {
  socklen_t addrLen = sizeof(address);

  clientFd = accept(serverFd, (struct sockaddr*)&address, &addrLen);
  if (clientFd < 0) return false;

  return true;
}

void ServerUtil::writeError() {
  write(clientFd, invalid_response, strlen(invalid_response));
}

bool ServerUtil::parseGetPath(const std::string& path,
                              std::vector<std::vector<char>>& grid) {
  // Path is of form "abcd,efgh,ijkl,mnop.
  // Split by commas into lines.
  std::vector<std::string> boardRowStrs;
  std::stringstream ss(path);
  std::string piece;
  while (std::getline(ss, piece, ',')) {
    boardRowStrs.push_back(piece);
  }

  // Verify that each row has the same number of characters (must be
  // rectangular).
  size_t rowSize = boardRowStrs[0].size();
  for (const auto& row : boardRowStrs) {
    if (row.size() != rowSize) {
      std::cout << "Invalid input, requires rectangular board.\n";
      return false;
    }
  }

  // Split the row chars into vectors, creating the 2d grid.
  for (const auto& row : boardRowStrs) {
    std::vector<char> rowChars;
    for (char c : row) {
      rowChars.push_back(c);
    }
    grid.push_back(rowChars);
  }

  return true;
}

bool ServerUtil::handleClient() {
  // Clear the buffer and read data from the client
  memset(buffer, 0, BUFFER_SIZE);

  std::cout << "Reading\n";

  // Read the HTTP request from the client.
  ssize_t bytes_read = read(clientFd, buffer, BUFFER_SIZE - 1);
  if (bytes_read <= 0) {
    writeError();
    close(clientFd);
    return false;
  }
  std::cout << "Done read.\n";

  std::string bufferStr(buffer, bytes_read);

  std::cout << "Message received: " << bufferStr << "\n";

  // Parse the first line (the GET request).
  std::size_t endOfLine = bufferStr.find("\r\n");
  if (endOfLine == std::string::npos) {
    std::cout << "Inavlid request (no first line found).\n";
    writeError();
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
    std::cout << "Invalid request line.\n";
    writeError();
    return false;
  }

  if (method != "GET") {
    std::cout << "Unsupported HTTP method: " << method << "\n";
    writeError();
    return false;
  }

  std::cout << "Path we got is: " << path << "\n";

  // Form the board based on the comma separated string.
  if (!path.empty() && path[0] == '/') {
    path = path.substr(1);  // remove the first character '/'
  }

  // Convert the path into a grid object, required to solve.
  std::vector<std::vector<char>> grid;
  if (!parseGetPath(path, grid)) {
    writeError();
    return false;
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

  // Write the result given the answer.
  std::string data = formatData(s.getFoundWords());
  std::cout << "Data string: " << data << "\n";


  std::string response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Access-Control-Allow-Origin: http://localhost:3000\r\n"
      "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
      "Content-Length: " + std::to_string(data.size()) + "\r\n"
      "\r\n" +
      data;

  write(clientFd, response.c_str(), response.size());

  std::cout << "Closing client socket.\n";
  close(clientFd);

  return true;
}

std::string ServerUtil::formatData(
    const std::vector<std::vector<std::pair<char, size_t>>>& data) {
  std::ostringstream oss;

  // Iterate through the outer vector
  for (size_t rowIndex = 0; rowIndex < data.size(); ++rowIndex) {
    const auto& row = data[rowIndex];

    // Iterate through the inner vector of (char, size_t) pairs
    for (size_t colIndex = 0; colIndex < row.size(); ++colIndex) {
      oss << row[colIndex].first << "," << row[colIndex].second;

      // Add a space if not the last pair in this row
      if (colIndex + 1 < row.size()) {
        oss << " ";
      }
    }

    // After finishing one row, add a "|"
    oss << "|";
  }

  return oss.str();
}

void ServerUtil::closeServer() { close(serverFd); }
