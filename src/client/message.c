#include "message.h"
#include <pthread.h>
#include <stdlib.h>

list_t message_list;

void init_message_list() {
  message_list.head = NULL;
  message_list.tail = NULL;
  message_list.size = 0;
  pthread_mutex_init(&message_list.mutex, NULL);
  pthread_cond_init(&message_list.cond, NULL);
}

void cleanup_message_list() {
  pthread_mutex_destroy(&message_list.mutex);
  pthread_cond_destroy(&message_list.cond);
}

node_t *new_node(message_packet_t *val) {
  node_t *node = malloc(sizeof(node_t));
  node->next = NULL, node->prev = NULL, node->val = val;
  return node;
}

void add_node(message_packet_t *val) {
  pthread_mutex_lock(&message_list.mutex);
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
  pthread_cond_signal(&message_list.cond);
  pthread_mutex_unlock(&message_list.mutex);
}
void pop_node() {
  pthread_mutex_lock(&message_list.mutex);
  while (message_list.size == 0) {
    pthread_cond_wait(&message_list.cond, &message_list.mutex);
  }
  if (message_list.size != 0) {
    node_t *temp = message_list.head;
    message_list.head = temp->next;
    message_list.head->prev = NULL;
    free(temp);
  }
  pthread_mutex_unlock(&message_list.mutex);
}
