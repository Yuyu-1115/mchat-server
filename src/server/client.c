#include "server/client.h"
#include "common/common.h"
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>

void init_cl() {
  cl.size = 0;
  for (size_t i = 0; i < MAX_CLIENT_NUM; ++i) {
    cl.clients[i] = -1;
  }
  pthread_mutex_init(&cl.mutex, NULL);
}
void cleanup_cl() { pthread_mutex_destroy(&cl.mutex); }

int32_t add_client(const int client_fd) {
  pthread_mutex_lock(&cl.mutex);
  if (cl.size == MAX_CLIENT_NUM) {
    pthread_mutex_unlock(&cl.mutex);
    return FAILED;
  }
  // the number is small, so linear search work just fine
  for (size_t i = 0; i < MAX_CLIENT_NUM; ++i) {
    if (cl.clients[i] == -1) {
      cl.clients[i] = client_fd;
      cl.size++;
      pthread_mutex_unlock(&cl.mutex);
      return SUCCESS;
    }
  }
  pthread_mutex_unlock(&cl.mutex);
  return FAILED;
}
int32_t remove_client(const int client_fd) {
  pthread_mutex_lock(&cl.mutex);
  for (size_t i = 0; i < MAX_CLIENT_NUM; ++i) {
    if (cl.clients[i] == client_fd) {
      cl.clients[i] = -1;
      cl.size--;
      pthread_mutex_unlock(&cl.mutex);
      return SUCCESS;
    }
  }
  pthread_mutex_unlock(&cl.mutex);
  return FAILED;
}

void broadcast(const int client_fd, const message_packet_t *packet) {
  pthread_mutex_lock(&cl.mutex);
  for (size_t i = 0; i < MAX_CLIENT_NUM; ++i) {
    if (cl.clients[i] != -1 && cl.clients[i] != client_fd) {
      if (send(cl.clients[i], packet, sizeof(message_packet_t), 0) == -1) {
        remove_client(cl.clients[i]);
      }
    }
  }
  pthread_mutex_unlock(&cl.mutex);
}
