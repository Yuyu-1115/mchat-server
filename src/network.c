#include "constant.h"
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int initialize_socket() {
  int s = socket(AF_INET, SOCK_STREAM, 0);

  if (s == -1) {
    handle_error_en(s, "Unable to create socket, aborting");
    return 0;
  }
  puts("Socket created");

  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT_NUMBER);
  memset(&addr.sin_addr, 0, sizeof(struct in_addr));

  size_t size_addr = sizeof(struct sockaddr_in);

  if (bind(s, (struct sockaddr *)&addr, size_addr) < 0) {
    handle_error("Failed to bind sockets.");
  }
  puts("Socket bound.");

  listen(s, 10);
  puts("Socket is listening and waiting for incoming connection...");
  return s;
}

void *handle_connection(void *client_s) {
  int s = *(int *)client_s;
  int read_size;
  char buffer[1024] = {0};
  char greeting[] = "Username: \n";

  printf("Accepting connection from thread %ld\n.", (long)pthread_self());

  send(s, greeting, sizeof(greeting), 0);

  while ((read_size = recv(s, buffer, sizeof(buffer), 0) > 0)) {
    send(s, buffer, sizeof(buffer), 0);
    puts(buffer);
  }

  close(s);

  return NULL;
}
