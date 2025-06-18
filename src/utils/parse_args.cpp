#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

struct ReplicaOptions {
  int port;
  std::string id;
  std::vector<std::string> otherIds;
};

ReplicaOptions parse_args(const int argc, char *argv[]) {
  ReplicaOptions opts = ReplicaOptions{};

  int c;

  static int verbose_flag;

  while (true) {
    static struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"brief", no_argument, &verbose_flag, 0},
        {"port", required_argument, 0, 'p'},
        {"id", required_argument, 0, 'i'}};

    int option_index;
    c = getopt_long(argc, argv, "i:p:", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 'p':
      opts.port = std::atoi(optarg);
      break;

    case 'i':
      opts.id = optarg;
      break;

    case '?':
      break;

    default:
      abort();
    }
  }

  while (optind < argc) {
    opts.otherIds.push_back(argv[optind]);
    optind++;
  }
  return opts;
}
