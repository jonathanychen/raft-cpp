#ifndef replica_h
#define replica_h

#include <nlohmann/json.hpp>
#include <replica/messages.h>
#include <string>
#include <sys/socket.h>
#include <vector>

using nlohmann::json;

struct ReplicaOptions {
  int port;
  std::string id;
  std::vector<std::string> otherIds;
};

class Replica {
private:
  // Initialization options
  int port;
  std::string id;
  std::vector<std::string> otherIds;

  // Connection information
  int conn;
  std::vector<int> peerConns;
  struct addrinfo *clientinfo;
  struct sockaddr_storage *simulator;
  socklen_t addr_size = sizeof simulator;

  int setup_sockets();
  int send_message(json message);
  json recv_message();

  // Message handlers
  void match_message_type(json message);
  void handle_get(GetRequest request);
  void handle_put(PutRequest request);

public:
  Replica(ReplicaOptions opts);
  void run();
};

#endif
