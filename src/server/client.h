#pragma once
#include "common/common.h"
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

enum { MAX_CLIENT_NUM = 10, FAILED = -1, SUCCESS = 0 };

typedef struct {
  int clients[MAX_CLIENT_NUM];
  size_t size;
  pthread_mutex_t mutex;
} client_list_t;

void init_cl();
void cleanup_cl();
int32_t add_client(const int client_fd);
int32_t remove_client(const int client_fd);
void broadcast(const message_packet_t *packet);
