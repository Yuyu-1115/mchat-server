#pragma once
#include <stdint.h>

enum {
  MAX_MESSAGE = 512,
  MAX_USERNAME = 32,
};

typedef enum { PKT_TYPE_JOIN, PKT_TYPE_CHAT, PKT_TYPE_EXIT } packet_type_t;

typedef struct {
  int32_t type;
  char username[MAX_USERNAME];
  char content[MAX_MESSAGE];
} message_packet_t;

int send_packet(int s, const message_packet_t *packet);
int recv_packet(int s, message_packet_t *packet);
void format_message(char *buffer, message_packet_t *packet);
