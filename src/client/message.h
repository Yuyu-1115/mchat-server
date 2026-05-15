#pragma once
#include "common/common.h"
#include <stddef.h>

struct node {
  struct node *next, *prev;
  message_packet_t *val;
};

typedef struct node node_t;

typedef struct {
  node_t *head, *tail;
  size_t size;
} list_t;

void init_message_list();
node_t *new_node(message_packet_t *val);
void add_node(message_packet_t *val);
void pop_node();
