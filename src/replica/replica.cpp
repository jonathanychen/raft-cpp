#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <replica/replica.h>
#include <sys/socket.h>
#include <sys/types.h>

static int BACKLOG_SIZE = 5;
static const char *LOCALHOST = "localhost";
static const char *ANY_PORT = "0";

Replica::Replica(ReplicaOptions opts) {
  port = opts.port;
  id = opts.id;
  otherIds = opts.otherIds;
}

// TODO: refactor this into smaller helper functions
void Replica::run() {
  // See Beej's guide for more in-depth explanations
  int status;
  struct addrinfo hints, *servinfo, *clientinfo;
  struct sockaddr_storage simulator;
  socklen_t addr_size = sizeof simulator;

  // Set properties for getaddrinfo usage
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  std::string portStr = std::to_string(port);

  // Get address info of both this replica, as well as the client (simulator)
  if ((status = getaddrinfo(LOCALHOST, portStr.c_str(), &hints, &clientinfo)) !=
      0) {
    fprintf(stderr, "gai client error: %s\n", gai_strerror(status));
  }
  if ((status = getaddrinfo(LOCALHOST, ANY_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "gai socket error: %s\n", gai_strerror(status));
    exit(1);
  }

  int sock =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if (sock == -1) {
    fprintf(stderr, "socket error: %s\n", strerror(errno));
    exit(1);
  }

  // Allow socket reusage
  int opt = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int binded = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);
  if (binded == -1) {
    fprintf(stderr, "bind error: %s\n", strerror(errno));
    exit(1);
  }

  freeaddrinfo(servinfo);

  std::string hello = "{\"src\": \"" + id +
                      "\", \"dst\": \"FFFF\", \"leader\": \"FFFF\", \"type\": "
                      "\"hello\", \"MID\": \"" +
                      id + "\"}";
  char const *helloCstr = hello.c_str();

  int bytes_sent;
  bytes_sent = sendto(sock, hello.c_str(), hello.length(), 0,
                      clientinfo->ai_addr, clientinfo->ai_addrlen);

  if (bytes_sent < 0) {
    std::cout << "error sending data" << std::endl;
    fprintf(stderr, "sendto error: %s\n", strerror(errno));
  } else if (bytes_sent > 0) {
    std::cout << "successfully sent message!" << std::endl;
  } else {
    std::cout << "something else happened" << std::endl;
  }

  char buffer[1024];
  int bytes_received;

  // Set read timeout
  // struct timeval tv;
  // tv.tv_sec = 10;
  // tv.tv_usec = 0;
  // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  while (true) {
    bytes_received = recvfrom(sock, buffer, sizeof buffer, 0,
                              (struct sockaddr *)&simulator, &addr_size);

    if (bytes_received < 0) {
      std::cout << "error receiving data" << std::endl;
      fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
    } else if (bytes_received == 0) {
      std::cout << "no data received, continuing" << std::endl;
    } else {
      std::cout << "received message!" << std::endl;
      ;
    }

    for (int i = 0; i < bytes_received; ++i) {
      std::cout << buffer[i];
    }
    std::cout << std::endl;
  }

  freeaddrinfo(clientinfo);
}
