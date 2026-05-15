#include "common/common.h"
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>

const char *template_chat = "[%s] %s\n";
const char *template_join = "%s has joined the chat.\n";
const char *template_exit = "%s has left the chat.\n";

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
int recv_packet(int s, message_packet_t *packet) {
  size_t curr_p = 0;
  int read_size = 0;
  while (curr_p < sizeof(message_packet_t)) {
    read_size = recv(s, packet + curr_p, sizeof(message_packet_t) - curr_p, 0);
    if (read_size < 0) {
      printf("Error during receving packets from clients: %s",
             gai_strerror(errno));
      return -1;
    }
    curr_p += read_size;
  }
  return 0;
}

void format_message(char *buffer, message_packet_t *packet) {
  switch (packet->type) {
  case PKT_TYPE_JOIN:
    sprintf(buffer, template_join, packet->username);
    break;
  case PKT_TYPE_EXIT:
    sprintf(buffer, template_exit, packet->username);
    break;
  case PKT_TYPE_CHAT:
    sprintf(buffer, template_chat, packet->username, packet->content);
    break;
  }
}
