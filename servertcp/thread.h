#ifndef SERVERTCP_THREAD_H
#define SERVERTCP_THREAD_H
#include "connection.h"
typedef struct {
  int conn_id;
  connection_t *connections;
} thr_arg;
#endif // SERVERTCP_THREAD_H
