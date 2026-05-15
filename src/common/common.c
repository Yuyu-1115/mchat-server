#include "common/common.h"
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

const char *template_chat = "[%s] %s\n";
const char *template_join = "%s has joined the chat.\n";
const char *template_exit = "%s has left the chat.\n";

int send_packet(int s, const message_packet_t *packet) {
  size_t curr_p = 0;
  ssize_t sent_size = 0;
  const char *buffer = (const char *)packet;
  while (curr_p < sizeof(message_packet_t)) {
    sent_size = send(s, buffer + curr_p, sizeof(message_packet_t) - curr_p, 0);
    if (sent_size <= 0) {
      if (sent_size < 0) {
        fprintf(stderr, "Error while sending packet: %s\n", strerror(errno));
      } else {
        fprintf(stderr, "Peer closed connection while sending packet.\n");
      }
      return -1;
    }
    curr_p += (size_t)sent_size;
  }
  return 0;
}
int recv_packet(int s, message_packet_t *packet) {
  size_t curr_p = 0;
  ssize_t read_size = 0;
  char *buffer = (char *)packet;
  while (curr_p < sizeof(message_packet_t)) {
    read_size = recv(s, buffer + curr_p, sizeof(message_packet_t) - curr_p, 0);
    if (read_size <= 0) {
      if (read_size < 0) {
        fprintf(stderr, "Error while receiving packet: %s\n", strerror(errno));
      } else {
        fprintf(stderr, "Peer closed connection while receiving packet.\n");
      }
      return -1;
    }
    curr_p += (size_t)read_size;
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
