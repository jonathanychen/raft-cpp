#ifndef parse_args_h
#define parse_args_h

#include <string>
#include <vector>

struct ReplicaOptions {
  int port;
  std::string id;
  std::vector<std::string> otherIds;
};

ReplicaOptions parse_args(int argc, char *argv[]);

#endif
