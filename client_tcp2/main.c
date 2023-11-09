#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
int cont = 0;

typedef struct {
  int socket_id;
  char name[10];
  int room_id;
} UserInfo;

void *p_send(void *ptr) {
  UserInfo *user_info;
  user_info = (UserInfo *)ptr;

  for (;;) {
    fflush(stdin);
    char buf[1000];
    fgets(buf, 1000, stdin);
    buf[strlen(buf) - 1] = '\0';

    if (strlen(buf) > 0) {
      int buf_len = strlen(buf);
      send(user_info->socket_id, &buf_len, sizeof(int), 0);
      send(user_info->socket_id, buf, sizeof(char) * buf_len, 0);
    }
  }

  return NULL;
}

void *p_recv(void *ptr) {
  UserInfo *user_info = (UserInfo *)ptr;

  for (;;) {
    long long history_size;
    recv(user_info->socket_id, &history_size, sizeof(long long), 0);

    char *history = (char *)malloc(history_size);
    recv(user_info->socket_id, history, history_size, 0);

    system("clear");
    printf("\n%s", history);
    free(history);
  }

  return NULL;
}

void chat_conn(int socket_id) {
  UserInfo *thread_info;
  pthread_t thread, thread_2, thread_3;

  thread_info = (UserInfo *)malloc(sizeof(UserInfo));
  thread_info->socket_id = socket_id;

  printf("Digite a Sala: ");
  scanf("%d", &thread_info->room_id);
  thread_info->room_id = thread_info->room_id;

  printf("Digite seu nome: ");
  fflush(stdin);
  fgets(thread_info->name, 10, stdin);
  thread_info->name[strlen(thread_info->name) - 1] = '\0';

  int name_len = strlen(thread_info->name);

  send(thread_info->socket_id, &thread_info->room_id, sizeof(int), 0);
  send(thread_info->socket_id, &name_len, sizeof(int), 0);
  send(thread_info->socket_id, thread_info->name, name_len, 0);
  fflush(stdin);

  pthread_create(&thread, 0, p_send, (void *)thread_info);
  pthread_create(&thread_2, 0, p_recv, (void *)thread_info);
  pthread_join(thread, NULL);
}

int main() {
  int socket_id;
  struct sockaddr_in addrport_cli;
  socket_id = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_id == -1) {
    printf("fail socket creation\n");
    exit(0);
  } else
    printf("sucess socket creation\n");

  bzero(&addrport_cli, sizeof(addrport_cli));

  addrport_cli.sin_family = AF_INET;
  addrport_cli.sin_port = htons(5100);
  addrport_cli.sin_addr.s_addr = htonl(INADDR_ANY);

  if (connect(socket_id, (struct sockaddr *)&addrport_cli,
              sizeof(addrport_cli)) != 0) {
    printf("connection failed\n");
    exit(0);
  } else
    printf("connection accept\n");
  printf("Escolha uma sala para Entrar\n");
  printf("Sala 1\n");
  printf("Sala 2\n");

  chat_conn(socket_id);

  close(socket_id);

  return 0;
}
