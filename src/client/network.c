#include "client/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int connect_to_host(const char *hostname, struct addrinfo hint,
                    struct addrinfo **res) {
  int s = -1;
  int status = getaddrinfo(hostname, "8080", &hint, res);
  if (status != 0) {
    fprintf(stderr, "Error during getting address info: %s\n",
            gai_strerror(status));
    exit(1);
  }

  for (struct addrinfo *p = *res; p != NULL; p = p->ai_next) {
    s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (s == -1) {
      continue;
    }

    if (connect(s, p->ai_addr, p->ai_addrlen) == 0) {
      break;
    }
    close(s);
    s = -1;
  }

  return s;
}
