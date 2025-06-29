#ifndef replica_h
#define replica_h

#include <string>
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

  int setup_sockets();

public:
  Replica(ReplicaOptions opts);
  void run();
};

#endif
