#include "common/common.h"
#include "common/message_queue.h"
#include "server/client.h"
#include <errno.h>
#include <netdb.h>
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
    printf("Failed to bind sockets., %s", gai_strerror(errno));
    close(s);
    exit(1);
  }
  puts("Socket bound.");

  listen(s, 10);
  puts("Socket is listening and waiting for incoming connection...");
  return s;
}

void *handle_connection(void *client_s) {
  int s = (int)(intptr_t)client_s;
  message_packet_t packet;
  message_packet_t test = {
      .type = PKT_TYPE_CHAT, .username = "Server", .content = "Welcome!"};

  add_client(s);
  send(s, &test, sizeof(message_packet_t), 0);
  while (1) {
    if (recv_packet(s, &packet) == -1) {
      break;
    }
    message_queue_push(&packet);
    printf("[%s]%s\n", packet.username, packet.content);
    if (packet.type == PKT_TYPE_EXIT) {
      break;
    }
  }

  remove_client(s);
  close(s);
  return NULL;
}

void *message_consumer_thread(__attribute__((unused)) void *arg) {
  while (1) {
    message_packet_t packet = message_queue_pop();
    broadcast(&packet);
  }
  return NULL;
}
