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
  int clientFd;
  struct sockaddr_in address;
  char buffer[BUFFER_SIZE];

  Solver::EnglishWordTrie trie;

  const char* pong_response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Access-Control-Allow-Origin: http://localhost:3000\r\n"
      "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
      "Content-Length: 4\r\n"
      "\r\n"
      "pong";

  // Bad request received.
  const char* invalid_response =
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

  bool acceptConn();

  bool handleClient();

  void closeServer();

 private:
  bool parseGetReq();
};

#endif  // __UTIL_H__
