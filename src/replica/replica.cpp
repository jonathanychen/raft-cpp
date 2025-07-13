#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <replica/messages.h>
#include <replica/replica.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unordered_map>

using json = nlohmann::json;

static int BACKLOG_SIZE = 5;
static const char *LOCALHOST = "localhost";
static const char *ANY_PORT = "0";

Replica::Replica(ReplicaOptions opts) {
  port = opts.port;
  id = opts.id;
  otherIds = opts.otherIds;
}

void Replica::run() {
  this->setup_sockets();

  json hello = {
      {"src", id},       {"dst", "FFFF"}, {"leader", "FFFF"},
      {"type", "hello"}, {"MID", id},
  };

  this->send_message(hello);

  while (true) {
    json message = this->recv_message();
    std::cout << message.dump(2) << std::endl;

    match_message_type(message);
  }

  freeaddrinfo(clientinfo);
}

int Replica::setup_sockets() {
  // See Beej's guide for more in-depth explanations
  int status;
  struct addrinfo hints, *servinfo;

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

  conn =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if (conn == -1) {
    fprintf(stderr, "socket error: %s\n", strerror(errno));
    exit(1);
  }

  // Allow socket reusage
  int opt = 1;
  setsockopt(conn, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int binded = bind(conn, servinfo->ai_addr, servinfo->ai_addrlen);
  if (binded == -1) {
    fprintf(stderr, "bind error: %s\n", strerror(errno));
    exit(1);
  }

  freeaddrinfo(servinfo);

  // Set read timeout
  struct timeval tv;
  tv.tv_sec = 10;
  tv.tv_usec = 0;
  setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  return 0;
}

int Replica::send_message(json message) {
  std::string message_str = message.dump();

  int bytes_sent;
  bytes_sent = sendto(conn, message_str.c_str(), message_str.length(), 0,
                      clientinfo->ai_addr, clientinfo->ai_addrlen);
  if (bytes_sent < 0) {
    std::cout << "error sending data" << std::endl;
    fprintf(stderr, "sendto error: %s\n", strerror(errno));
  } else if (bytes_sent > 0) {
    std::cout << "successfully sent message!" << std::endl;
    std::cout << message.dump(2);
  } else {
    std::cout << "something else happened" << std::endl;
  }

  return bytes_sent;
}

json Replica::recv_message() {
  char buffer[1024];
  int bytes_received;

  bytes_received = recvfrom(conn, buffer, sizeof buffer, 0,
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

  std::string result(buffer, bytes_received);

  json message = json::parse(result);

  return message;
}

void Replica::match_message_type(json message) {
  std::string type = message["type"];

  std::unordered_map<std::string, std::function<void()>> commands = {
      {"get",
       [message, this]() {
         GetRequest request = message.template get<GetRequest>();
         this->handle_get(request);
       }},
      {"put", [message, this]() {
         PutRequest request = message.template get<PutRequest>();
         this->handle_put(request);
       }}};

  commands[type]();
}

void Replica::handle_get(GetRequest request) {}

void Replica::handle_put(PutRequest request) {
  PutFailResponse response = {request.dst, request.src, id, request.MID,
                              "fail"};

  json message = response;

  this->send_message(message);
}
