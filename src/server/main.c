#include "common/message_queue.h"
#include "network.h"
#include "server/client.h"
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void init() {
  // prevent SIGPIP from killing the process
  pthread_t message_queue;
  signal(SIGPIPE, SIG_IGN);
  init_mq();
  init_cl();
  if (pthread_create(&message_queue, NULL, message_consumer_thread, NULL) !=
      0) {
    perror("Error during creation of message queue");
    exit(1);
  }
}

// TODO: more proper clean up such as closing other thread gracefully is needed
void wrap_up() {
  cleanup_mq();
  cleanup_cl();
}

int main() {
  int client_fd;
  pthread_t client;
  int s = initialize_socket();
  init();
  while ((client_fd = accept(s, 0, 0))) {
    if (pthread_create(&client, NULL, handle_connection,
                       (void *)(intptr_t)client_fd) != 0) {
      perror("Error during thread creation when handling incoming connection");
    } else {
      pthread_detach(client);
    }
  }
  close(s);
  wrap_up();
  return 0;
}
