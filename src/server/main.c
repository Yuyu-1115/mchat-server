#include "network.h"
#include "server/client.h"
#include "server/message_queue.h"
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void init() {
  // prevent SIGPIP from killing the process
  signal(SIGPIPE, SIG_IGN);
  init_mq();
  init_cl();
}

void wrap_up() {
  cleanup_mq();
  cleanup_cl();
}

int main() {
  int client_fd;
  pthread_t c;
  int s = initialize_socket();
  init();
  while ((client_fd = accept(s, 0, 0))) {
    if ((c = pthread_create(&c, NULL, handle_connection, (void *)&client_fd) !=
             0)) {
      perror("Error during thread creation");
    }
  }
  close(s);
  wrap_up();
  return 0;
}
