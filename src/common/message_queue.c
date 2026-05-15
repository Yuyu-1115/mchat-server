#include "common/message_queue.h"
#include "common/common.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static message_queue_t mq;

void init_mq() {
  mq.size = 0;
  mq.head = 0;
  mq.tail = 0;
  if (pthread_mutex_init(&mq.mutex, NULL) != 0) {
    puts("Fail to create mutex when initializing message queue");
    exit(1);
  }
  if (pthread_cond_init(&mq.cond, NULL) != 0) {
    puts("Fail to create conditional when initializing message queue");
    exit(1);
  }
}

void cleanup_mq() {
  pthread_mutex_destroy(&mq.mutex);
  pthread_cond_destroy(&mq.cond);
}

void message_queue_push(const message_packet_t *packet) {
  pthread_mutex_lock(&mq.mutex);
  if (mq.size == MESSAGE_QUEUE_SIZE) {
    pthread_mutex_unlock(&mq.mutex);
    return;
  }
  mq.queue[mq.head] = *packet;
  mq.head = (mq.head + 1) % MESSAGE_QUEUE_SIZE;
  ++mq.size;
  pthread_mutex_unlock(&mq.mutex);
  pthread_cond_signal(&mq.cond);
}

message_packet_t message_queue_pop() {
  pthread_mutex_lock(&mq.mutex);
  message_packet_t packet = {0};
  while (mq.size == 0) {
    pthread_cond_wait(&mq.cond, &mq.mutex);
  }
  packet = mq.queue[mq.tail];
  mq.tail = (mq.tail + 1) % MESSAGE_QUEUE_SIZE;
  --mq.size;
  pthread_mutex_unlock(&mq.mutex);
  return packet;
}
