#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <replica/replica.h>
#include <sys/socket.h>

Replica::Replica(ReplicaOptions opts) {
  port = opts.port;
  id = opts.id;
  otherIds = opts.otherIds;
}

int Replica::run() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  int binded = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if (binded != 0) {
    return 1;
  }

  int listening = listen(sock, 5);
  if (listening != 0) {
    return 1;
  }

  // int client = accept(sock, nullptr, nullptr);

  //  char buffer[1024];
  //  ssize_t bytes_received;
  //
  //  while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
  //    std::cout << buffer;
  //  }

  return 0;
}
