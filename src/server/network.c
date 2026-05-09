#include "common/common.h"
#include "server/client.h"
#include "server/message_queue.h"
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT_NUMBER 8080

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
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
    puts("Failed to bind sockets.");
    close(s);
    exit(1);
  }
  puts("Socket bound.");

  listen(s, 10);
  puts("Socket is listening and waiting for incoming connection...");
  return s;
}

void *handle_connection(void *client_s) {
  int s = *(int *)client_s;
  int read_size;
  message_packet_t packet;
  char *buffer = (char *)&packet;

  while (1) {
    size_t curr_p = 0;
    while (curr_p < sizeof(message_packet_t)) {
      read_size = recv(s, buffer + curr_p, sizeof(buffer) - curr_p, 0);
      if (read_size < 0) {
        puts("Error during receving packets from clients");
        goto connection_closed;
      }
      curr_p += read_size;
    }
    message_queue_push(&packet);
  }

connection_closed:
  remove_client(s);
  close(s);
  return NULL;
}
