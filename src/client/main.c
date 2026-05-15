#include "client/context.h"
#include "client/message.h"
#include "client/tui.h"
#include "common/common.h"
#include "common/message_queue.h"
#include "network.h"
#include <_time.h>
#include <ncurses.h>
#include <netdb.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tui_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t comm_thread;

void init(const char *username) {
  init_mq();
  init_message_list();
  init_tui(username);
}

void *communicate_thread(void *server_fd) {
  int s = (int)(intptr_t)server_fd;
  message_packet_t packet;
  while (1) {
    packet = message_queue_pop();
    send_packet(s, &packet);
  }
}

int main(int arg, char *argv[]) {
  if (arg < 3) {
    puts("Usage: mchat-client <hostname> <username>");
  }

  char *username = argv[2];
  message_packet_t packet, buffer;
  struct addrinfo hint, *res;
  char message_buffer[sizeof(message_packet_t)] = {0};

  init(username);

  memcpy(packet.content, "Hello", sizeof("Hello"));

  if (strlen(username) > MAX_USERNAME) {
    printf("Username is too long, the limit is %d bytes.", MAX_USERNAME);
  }

  memset(&hint, 0, sizeof(struct addrinfo));
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;

  int s = connect_to_host(argv[1], hint, &res);
  if (s == -1) {
    endwin();
    fprintf(stderr, "Unable to connect to %s\n", argv[1]);
    exit(1);
  } else {
    printf("Successfully connected to %s\n", argv[1]);
  }

  pthread_create(&comm_thread, NULL, communicate_thread, (void *)(intptr_t)s);

  while (1) {
    recv_packet(s, &packet);
    pthread_mutex_lock(&data_mutex);
    add_node(&packet);
    format_message(message_buffer, &packet);
    pthread_mutex_unlock(&data_mutex);
    printf("[%s] %s\n", buffer.username, buffer.content);
  }

  freeaddrinfo(res);
  endwin();
  return 0;
}
