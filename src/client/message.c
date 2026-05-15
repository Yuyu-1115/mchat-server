#include "message.h"
#include <stdlib.h>

list_t message_list;

void init_message_list() {
  message_list.head = NULL;
  message_list.tail = NULL;
  message_list.size = 0;
}

node_t *new_node(message_packet_t *val) {
  node_t *node = malloc(sizeof(node_t));
  node->next = NULL, node->prev = NULL, node->val = val;
  return node;
}

void add_node(message_packet_t *val) {
  node_t *node = new_node(val);
  if (message_list.size == 0) {
    message_list.head = node;
    message_list.tail = node;
  } else {
    node_t *temp = message_list.tail;
    message_list.tail = node;
    temp->next = node;
    node->prev = temp;
  }
}
void pop_node() {
  if (message_list.size == 0) {
    return;
  } else {
    node_t *temp = message_list.head;
    message_list.head = temp->next;
    message_list.head->prev = NULL;
    free(temp);
  }
}
