#pragma once

#include <netdb.h>
int connect_to_host(const char *hostname, struct addrinfo hint,
                    struct addrinfo **res);
