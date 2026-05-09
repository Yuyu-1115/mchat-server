#pragma once
#include "common/common.h"
#include <bits/pthreadtypes.h>
#include <stddef.h>

enum { MESSAGE_QUEUE_SIZE = 100 };

typedef struct {
  message_packet_t queue[MESSAGE_QUEUE_SIZE];
  size_t head;
  size_t tail;
  size_t size;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} message_queue_t;

static message_queue_t mq;

void init_mq();
void cleanup_mq();
void message_queue_push(const message_packet_t *packet);
message_packet_t message_queue_pop();
