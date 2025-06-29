#include "replica/replica.h"
#include "utils/parse_args.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << " ";
  }

  std::cout << '\n';

  ReplicaOptions opts = parse_args(argc, argv);

  // 1. Parse main arguments and place into options struct
  std::cout << "Parsed arguments!\n";

  Replica server = Replica(opts);

  // 2. Start replica loop
  std::cout << "Replica running.\n";

  server.run();

  return EXIT_SUCCESS;
}
