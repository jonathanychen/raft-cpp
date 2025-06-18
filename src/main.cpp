#include "utils/parse_args.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::cout << "Number of arguments: " << argc << '\n';

  for (int i = 0; i < argc; ++i) {
    std::cout << "Argument " << i << ": " << argv[i] << '\n';
  }

  std::cout << '\n';

  ReplicaOptions opts = parse_args(argc, argv);

  std::cout << "Port: " << opts.port << '\n';
  std::cout << "Id: " << opts.id << '\n';

  std::cout << "OtherIDs: ";

  for (std::string otherId : opts.otherIds) {
    std::cout << otherId << ' ';
  }
  std::cout << '\n';

  // 1. Parse main arguments and place into options struct
  std::cout << "Parsed arguments!\n";

  // 2. Start replica loop
  std::cout << "Replica running.\n";

  return EXIT_SUCCESS;
}
