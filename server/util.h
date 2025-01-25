#ifndef __UTIL_H__
#define __UTIL_H__

#include <tuple>

#include "solver.h"
#include <arpa/inet.h>

#define MAX_PORT_NO 99999
#define BUFFER_SIZE 1024

struct ServerUtil {
  unsigned int port;

  int serverFd;
  struct sockaddr_in address;
  char buffer[BUFFER_SIZE];

  Solver::EnglishWordTrie trie;

  const char* validResponsePrefix =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Access-Control-Allow-Origin: http://localhost:3000\r\n"
      "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";

  // Bad request received.
  const char* invalidResponse =
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-Type: text/plain\r\n"
      "Access-Control-Allow-Origin: http://localhost:3000\r\n"
      "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
      "Content-Length: 14\r\n"
      "\r\n"
      "Invalid input.";

  ServerUtil(unsigned int port_);

  // Returns the server file descriptor, or a true boolean if there's an error.
  // Also sets socket options to reuse address after fd closes (program ends).
  bool openTcp();

  bool bindToPort();

  bool listenConn();

  std::pair<bool, int> acceptConn();

  bool handleClient(int clientFd);

  void closeServer();

 private:
  bool parseGetPath(const std::string& path, std::vector<std::vector<char>>& grid);

  void writeResponse(int clientFd, const char* response);

  std::string formatData(const std::vector<std::vector<std::pair<char, size_t>>>& data);
};

#endif  // __UTIL_H__
