#include "network.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int client_fd;
  pthread_t c;
  int s = initialize_socket();
  while ((client_fd = accept(s, 0, 0))) {
    if ((c = pthread_create(&c, NULL, handle_connection, (void *)&client_fd) !=
             0)) {
      perror("Error during thread creation");
    }
  }

  close(s);

  return 0;
}
