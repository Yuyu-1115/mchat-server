#pragma once
int initialize_socket();
void *handle_connection(void *client_s);
void *message_consumer_thread(__attribute__((unused)) void *arg);
