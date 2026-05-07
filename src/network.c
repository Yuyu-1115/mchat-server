#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void *handle_connection(void *client_s) {
  int s = *(int *)client_s;
  int read_size;
  char buffer[1024] = {0};
  char greeting[] = "Username: \n";
  char bye[] = "Bye: \n";

  printf("Accepting connection from thread %ld\n.", (long)pthread_self());

  send(s, greeting, sizeof(greeting), 0);

  while ((read_size = recv(s, buffer, sizeof(buffer), 0) > 0)) {
  }

  send(s, bye, sizeof(bye), 0);
  send(s, buffer, sizeof(buffer), 0);
  close(s);

  return NULL;
}
