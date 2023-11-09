#ifndef SERVERTCP_CONNECTION_H
#define SERVERTCP_CONNECTION_H
#include <pthread.h>
#include <sys/socket.h>

typedef struct {
  char name[10];
  int room_id;

  int socket_id;
  struct sockaddr address;

  pthread_t client_thread;
} connection_t;

#endif // SERVERTCP_CONNECTION_H
