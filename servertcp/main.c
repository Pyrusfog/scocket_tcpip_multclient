#include "connection.h"
#include "thread.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// =========================================================================
// ROOM HANDLING
// =========================================================================
#define EMPTY_ROOM_SLOT -1
#define MAX_ROOM_NUMBER 2
#define MAX_ROOM_MEMBERS 3

pthread_mutex_t rooms_mutex = PTHREAD_MUTEX_INITIALIZER;
int rooms[MAX_ROOM_NUMBER][MAX_ROOM_MEMBERS];

void initialize_room_ids() {
  pthread_mutex_lock(&rooms_mutex);
  for (int i = 0; i < MAX_ROOM_NUMBER; i++)
    for (int j = 0; j < MAX_ROOM_MEMBERS; j++) {
      rooms[i][j] = EMPTY_ROOM_SLOT;
    }
  pthread_mutex_unlock(&rooms_mutex);
}

void add_room_user(int room_id, int conn_id) {
  pthread_mutex_lock(&rooms_mutex);
  if (conn_id == EMPTY_ROOM_SLOT) {
    pthread_mutex_unlock(&rooms_mutex);
    // SHOULD NEVER GET HERE
    printf("[ERROR] INVALID_CONN_ID\n");
    exit(1);
  }

  for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
    if (rooms[room_id][i] == EMPTY_ROOM_SLOT) {
      rooms[room_id][i] = conn_id;
      break;
    }
  }
  pthread_mutex_unlock(&rooms_mutex);
}

void remove_room_user(int room_id, int conn_id) {
  pthread_mutex_lock(&rooms_mutex);
  if (conn_id == EMPTY_ROOM_SLOT) {
    return;
  }

  for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
    if (rooms[room_id][i] == conn_id) {
      rooms[room_id][i] = EMPTY_ROOM_SLOT;
    }
  }
  pthread_mutex_unlock(&rooms_mutex);
}

void show_rooms_layout() {
  pthread_mutex_lock(&rooms_mutex);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ", rooms[i][j]);
    }
    printf("\n");
  }
  pthread_mutex_unlock(&rooms_mutex);
}

// =========================================================================
// HISTORY HANDLING
// =========================================================================
#define HISTORY_STEP_SIZE 3000

pthread_mutex_t history_mutex = PTHREAD_MUTEX_INITIALIZER;
long long history_chat_sizes[MAX_ROOM_NUMBER];
char *history_chat[MAX_ROOM_NUMBER];

void initialize_history() {
  pthread_mutex_lock(&history_mutex);
  for (int i = 0; i < MAX_ROOM_NUMBER; i++) {
    history_chat[i] = (char *)malloc(sizeof(char) * HISTORY_STEP_SIZE);
    history_chat_sizes[i] = HISTORY_STEP_SIZE;
  }
  pthread_mutex_unlock(&history_mutex);
}

void append_to_history(int room, char *author_name, char *message) {
  pthread_mutex_lock(&history_mutex);
  long long *history_size = &history_chat_sizes[room];
  if (strlen(author_name) + strlen(message) + strlen(": ") + 1 /* new line */ >
      *history_size) {
    *history_size += HISTORY_STEP_SIZE;
    history_chat[room] = realloc(history_chat[room], *history_size);
  }

  strcat(history_chat[room], author_name);
  strcat(history_chat[room], ": ");
  strcat(history_chat[room], message);
  strcat(history_chat[room], "\n");
  pthread_mutex_unlock(&history_mutex);
}

// =========================================================================
// CONNECTION HANDLING
// =========================================================================
#define MAX_CONN 5

pthread_mutex_t connections_mutex = PTHREAD_MUTEX_INITIALIZER;

int is_sock_up(int socket_id) {
  return recv(socket_id, NULL, 1, MSG_PEEK | MSG_DONTWAIT) != 0;
}

void p_send(thr_arg *arg_thread, char buf[1000]) {
  int i = 0;

  pthread_mutex_lock(&connections_mutex);
  int conn_id = arg_thread->conn_id;
  connection_t conn = arg_thread->connections[conn_id];
  pthread_mutex_unlock(&connections_mutex);

  int room_id = conn.room_id;
  append_to_history(room_id, conn.name, buf);

  for (i = 0; i < MAX_ROOM_MEMBERS; i++) {
    if (strlen(buf) > 0 && rooms[room_id][i] != EMPTY_ROOM_SLOT) {
      pthread_mutex_lock(&connections_mutex);
      int socket_id = arg_thread->connections[rooms[room_id][i]].socket_id;
      pthread_mutex_unlock(&connections_mutex);

      send(socket_id, &history_chat_sizes[room_id], sizeof(long long), 0);
      send(socket_id, history_chat[room_id], history_chat_sizes[room_id], 0);
    }
  }
}

void *p_recv(void *ptr) {
  if (!ptr) {
    pthread_exit(0);
  }

  thr_arg *arg_thread = (thr_arg *)ptr;
  pthread_mutex_lock(&connections_mutex);
  int socket_id = arg_thread->connections[arg_thread->conn_id].socket_id;
  pthread_mutex_unlock(&connections_mutex);

  int room_id;
  int name_len;
  char name[10];

  recv(socket_id, &room_id, sizeof(int), 0);
  recv(socket_id, &name_len, sizeof(int), 0);
  recv(socket_id, name, name_len, 0);

  room_id -= 1;
  if (room_id > MAX_ROOM_NUMBER - 1) {
    room_id = MAX_ROOM_NUMBER - 1;
  } else if (room_id < 0) {
    room_id = 0;
  }

  add_room_user(room_id, arg_thread->conn_id);
  show_rooms_layout();
  printf("\nNovo Usuário: [%d] %s\n", room_id, name);
  fflush(stdout);

  pthread_mutex_lock(&connections_mutex);
  connection_t *conn = &arg_thread->connections[arg_thread->conn_id];
  strcpy(conn->name, name);
  conn->room_id = room_id;
  pthread_mutex_unlock(&connections_mutex);

  while (is_sock_up(socket_id)) {
    int buff_len;
    recv(socket_id, &buff_len, sizeof(int), 0);

    if (buff_len <= 0) {
      continue;
    }

    char *buff = (char *)malloc(sizeof(char) * buff_len);
    recv(socket_id, buff, buff_len, 0);
    printf("\n[%d] %s: \"%s\"", conn->room_id, name, buff);
    p_send(arg_thread, buff);

    free(buff);
  }

  {
    printf("\nConn %d ended :: Cleaning room information...\n",
           arg_thread->conn_id);
    remove_room_user(room_id, arg_thread->conn_id);

    printf("Conn %d ended :: Cleaning connection information...\n\n",
           arg_thread->conn_id);

    pthread_mutex_lock(&connections_mutex);
    connection_t conn = arg_thread->connections[arg_thread->conn_id];
    close(conn.socket_id);
    arg_thread->connections[arg_thread->conn_id].socket_id = -1;
    pthread_mutex_unlock(&connections_mutex);
  }

  return NULL;
}

int main() {
  initialize_room_ids();
  initialize_history();

  // =========================================================================
  // SOCKET BOILERPLATE
  // =========================================================================
  struct sockaddr_in servaddr;

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    printf("falhou na criacao do socket\n");
    exit(0);
  }

  int on_state = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on_state,
                 sizeof(on_state))) {
    printf("falhou ao configurar socket\n");
    exit(0);
  }

  printf("criou com sucesso\n");
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(5100);

  if ((bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind fail\n");
    exit(0);
  }
  printf("socket bind success\n");

  if ((listen(socket_fd, 5)) != 0) {
    printf("listen fail \n");
    exit(0);
  }
  printf("listen success\n");

  // =========================================================================
  // CLIENT HANDLING
  // =========================================================================
  int current_conn_idx = 0;
  int i = 0;
  connection_t connections[MAX_CONN];
  memset(connections, 0, sizeof(connection_t) * MAX_CONN);

  while (1) {
    connection_t *conn = &connections[current_conn_idx];
    if (conn->socket_id > 0) {
      current_conn_idx = (current_conn_idx + 1) % MAX_CONN;
      continue;
    }

    conn->socket_id = accept(socket_fd, NULL, NULL);
    if (conn->socket_id <= 0) {
      continue;
    }
    printf("client accept\n");

    printf("conn id : %i\n", current_conn_idx);

    thr_arg *arg = (thr_arg *)malloc(sizeof(thr_arg));
    arg->connections = connections;
    arg->conn_id = current_conn_idx;

    pthread_create(&conn->client_thread, 0, p_recv, (void *)arg);
    pthread_detach(conn->client_thread);
    current_conn_idx = (current_conn_idx + 1) % MAX_CONN;
  }
}
