#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <vector>

#include "replica/replica.h"

ReplicaOptions parse_args_opts(const int argc, char *argv[]) {
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
      opts.id = std::atoi(optarg);
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

ReplicaOptions parse_args(const int argc, char *argv[]) {
  ReplicaOptions opts = ReplicaOptions{};

  if (argc < 4) {
    throw;
  }

  for (int i = 1; i < argc; ++i) {
    int num = std::atoi(argv[i]);

    if (i == 1) {
      opts.port = num;
    } else if (i == 2) {
      opts.id = argv[i];
    } else {
      opts.otherIds.push_back(argv[i]);
    }
  }

  return opts;
}
