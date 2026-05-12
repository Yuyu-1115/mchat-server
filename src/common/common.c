#include "common/common.h"
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>

void send_packet(int s, message_packet_t *packet) {
  size_t curr_p = 0;
  int read_size = 0;
  while (curr_p < sizeof(message_packet_t)) {
    read_size = send(s, packet + curr_p, sizeof(message_packet_t) - curr_p, 0);
    if (read_size < 0) {
      printf("Error during receving packets from clients: %s",
             gai_strerror(errno));
    }
    curr_p += read_size;
  }
}
void recv_packet(int s, message_packet_t *packet) {
  size_t curr_p = 0;
  int read_size = 0;
  while (curr_p < sizeof(message_packet_t)) {
    read_size = recv(s, packet + curr_p, sizeof(message_packet_t) - curr_p, 0);
    if (read_size < 0) {
      printf("Error during receving packets from clients: %s",
             gai_strerror(errno));
    }
    curr_p += read_size;
  }
}
