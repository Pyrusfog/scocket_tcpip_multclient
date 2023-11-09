#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
int cont =0;

typedef struct
{
    int socket_id;
    char buff[1000];
    char name[10];
    char room_id[5];
} UserInfo;

void *console_fgets(void *ptr) {
    UserInfo *user_info;
    if (!ptr) pthread_exit(0);
    user_info = (UserInfo *)ptr;
    pthread_mutex_lock(&send_mutex);
    if (cont==0){
        printf("Digite a Sala: ");
        fgets(user_info->room_id, sizeof(user_info->room_id), stdin);
         cont++;
    }
    pthread_mutex_unlock(&send_mutex);
    while (1) {
        pthread_mutex_lock(&send_mutex);
        fgets(user_info->buff, sizeof(user_info->buff), stdin);
        pthread_mutex_unlock(&send_mutex);

        if (user_info->buff[0] != '\n'){
            break;
        }
    }
}

void * p_send(void *ptr){
    UserInfo *user_info;
    user_info = (UserInfo *)ptr;
    pthread_t thread;
    pthread_create(&thread, 0, console_fgets, (void *) user_info);
    pthread_mutex_lock(&send_mutex);

    if (user_info->buff[0] != 0) {
        if (user_info->buff[0] != '\n') {
            strcpy(user_info->name, "Jansen");
            send(user_info->socket_id, user_info->name, sizeof(user_info->name), 0);
            send(user_info->socket_id, user_info->buff, sizeof(user_info->buff), 0);
            send(user_info->socket_id, user_info->room_id, sizeof(user_info->room_id), 0);
            
        }
    }
    bzero(user_info->buff, sizeof (user_info->buff));
    bzero(user_info->name, sizeof (user_info->name));
    pthread_mutex_unlock(&send_mutex);
}

void * p_recv(void *ptr){
    UserInfo *user_info;
    if (!ptr) pthread_exit(0);
    user_info = (UserInfo *)ptr;
    bzero(user_info->buff, sizeof (user_info->buff));
    bzero(user_info->name, sizeof (user_info->name));

    recv(user_info->socket_id, user_info->buff, sizeof (user_info->buff), 0);
    if (user_info->buff[0] != 0) {
        if (user_info->buff[0] != '\n') {
            system("clear");
            printf("\n%s", user_info->buff);
        }
    }
    bzero(user_info->buff, sizeof (user_info->buff));
    bzero(user_info->name, sizeof (user_info->name));
}

void chat_conn(int socket_id){
    UserInfo *thread_send, *thread_recv;
    pthread_t thread,thread_2;

    thread_send = (UserInfo *) malloc(sizeof(UserInfo));
    thread_recv = (UserInfo *) malloc(sizeof(UserInfo));
    thread_send->socket_id = socket_id;
    thread_recv->socket_id = socket_id;

    for (;;) {
        pthread_create(&thread,0,p_send,(void *)thread_send);
        pthread_create(&thread_2,0,p_recv,(void *)thread_recv);
        pthread_join(thread, NULL);
    }
}

int main() {
    int socket_id;
    struct sockaddr_in addrport_cli;
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_id == -1){
        printf("fail socket creation\n");
        exit(0);
    }else
        printf("sucess socket creation\n");

    bzero(&addrport_cli,sizeof(addrport_cli));

    addrport_cli.sin_family = AF_INET;
    addrport_cli.sin_port = htons(5100);
    addrport_cli.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(socket_id, (struct sockaddr*)&addrport_cli,sizeof(addrport_cli)) != 0){
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
