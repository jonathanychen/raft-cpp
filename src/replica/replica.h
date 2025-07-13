#ifndef replica_h
#define replica_h

#include <string>
#include <sys/socket.h>
#include <vector>

struct ReplicaOptions {
  int port;
  std::string id;
  std::vector<std::string> otherIds;
};

class Replica {
private:
  int port;
  std::string id;
  std::vector<std::string> otherIds;

  int conn;
  std::vector<int> peerConns;
  struct addrinfo *clientinfo;
  struct sockaddr_storage *simulator;
  socklen_t addr_size = sizeof simulator;

  int setup_sockets();
  int send_message(std::string message);
  std::string recv_message();

public:
  Replica(ReplicaOptions opts);
  void run();
};

#endif
