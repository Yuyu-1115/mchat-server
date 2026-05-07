#include "constant.h"
#include "network.h"
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char buffer[1024] = {0};
  char response[] = "Hello, World";

  int s = socket(AF_INET, SOCK_STREAM, 0);
  int client_fd;

  if (s == -1) {
    printf("Unable to create socket, aborting...\n");
    return 0;
  }
  puts("Socket created");

  const uint16_t port = htons(PORT_NUMBER);
  struct sockaddr_in addr = {AF_INET, port, 0};
  size_t size_addr = sizeof(struct sockaddr_in);

  if (bind(s, (struct sockaddr *)&addr, size_addr) < 0) {
    perror("Failed to bind sockets.");
    return 0;
  }
  puts("Bind socket.");

  listen(s, 10);

  puts("Waiting for incoming connection...");
  pthread_t c;
  while ((client_fd = accept(s, 0, 0))) {
    if ((c = pthread_create(&c, NULL, handle_connection, (void *)&client_fd) !=
             0)) {
      perror("Error during thread creation");
    }
  }

  close(s);

  return 0;
}
