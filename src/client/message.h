#pragma once
#include "common/common.h"
#include <stddef.h>
#include <sys/_pthread/_pthread_cond_t.h>
#include <sys/_pthread/_pthread_mutex_t.h>

struct node {
  struct node *next, *prev;
  message_packet_t *val;
};

typedef struct node node_t;

typedef struct {
  node_t *head, *tail;
  size_t size;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} list_t;

void init_message_list();
node_t *new_node(message_packet_t *val);
void add_node(message_packet_t *val);
void pop_node();
