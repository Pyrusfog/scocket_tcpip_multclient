#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "connection.h"
#include "thread.h"

pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
char history_chat[2][3000];
int mat[2][3] = {777,777,777,777,777,777};
int cont = 0;

void room_ids(int value_room_id,int conn_id){
    int i,j;
    for(i=0;i<2;i++)
        for(j=0;j<3;j++)
        {
            if (mat[i][j] == conn_id){
                mat[i][j] = 777;
            }
        }
    for (i = value_room_id; i == value_room_id; i++) {
        for (j = 0; j < 3; j++) {
            if (mat[i][j] == 777) {
                mat[i][j] = conn_id;
                break;
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }

}

void * p_send(void *ptr){
    int i,int_id_room=0;
    thr_arg *arg_thread;
    arg_thread = (thr_arg *)ptr;

    pthread_mutex_lock(&send_mutex);
    int_id_room = atoi(arg_thread->id_room);

    strcat(arg_thread->name," :");
    strcat(arg_thread->name,arg_thread->buff);
    strcat(history_chat[int_id_room],arg_thread->name);
    pthread_mutex_unlock(&send_mutex);
    room_ids(int_id_room,arg_thread->socket_id);

    for(i=0; i<2; i++) {
        if (arg_thread->buff[0] != '\n') {
            pthread_mutex_lock(&send_mutex);

            if (mat[int_id_room][i] != 777){
                send(mat[int_id_room][i], history_chat[int_id_room], strlen(history_chat[int_id_room]), 0);
            }
            pthread_mutex_unlock(&send_mutex);
        }
    }
    bzero(arg_thread->buff,sizeof (arg_thread->buff));
    bzero(arg_thread->name,sizeof (arg_thread->name));
    bzero(arg_thread->id_room,sizeof (arg_thread->id_room));
}


void * p_recv(void *ptr){
    thr_arg *arg_thread;
    pthread_t thread_2;

    if (!ptr) pthread_exit(0);
    arg_thread = (thr_arg *)ptr;
    recv(arg_thread->socket_id,arg_thread->name,10,0);
    recv(arg_thread->socket_id,arg_thread->buff,1000,0);
    recv(arg_thread->socket_id,arg_thread->id_room, 5,0);
    pthread_create(&thread_2,0,p_send,(void *)arg_thread);

    if (arg_thread->buff[0] != 0){
        printf("\n%s: %s",arg_thread->name,arg_thread->buff);
        printf("\nid rooooooooooooooom%s\n",arg_thread->id_room);
    }
}

void * process(void * ptr) {
    int i;
    connection_t *conn;
    thr_arg *arg_thread;
    pthread_t thread;
    pthread_t thread_2;

    if (!ptr) pthread_exit(0);
    conn = (connection_t *)ptr;
    arg_thread = (thr_arg *) malloc(sizeof(thr_arg));

    for(i=0; i<5; i++){
        arg_thread->id_conn[i] = conn->id_conn[i];

    }
    bzero(arg_thread->buff,sizeof (arg_thread->buff));
    bzero(arg_thread->name,sizeof (arg_thread->name));

    while (1){
        arg_thread->socket_id = conn->socket_id;
        pthread_create(&thread,0,p_recv,(void *)arg_thread);

        if(strcmp(arg_thread->buff,"exit") == 0){
            printf("disconnect");
            break;
        }
        pthread_join(thread, NULL);
        pthread_join(thread_2, NULL);
    }

    close(conn->socket_id);
    free(conn);
    pthread_exit(0);

}
int main() {
    int socket_fd, len;
    struct sockaddr_in servaddr,cli;
    connection_t * connection;
    pthread_t thread;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        printf("falhou na criacao do socket\n");
        exit(0);
    }
    else
        printf("criou com sucesso\n");
    bzero(&servaddr,sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(5100);

    if ((bind(socket_fd, (struct sockaddr* )&servaddr, sizeof(servaddr))) != 0){
        printf("socket bind fail\n");
        exit(0);
    } else
        printf("sokcet bind sucess\n");

    if((listen(socket_fd, 5)) != 0){
        printf("listen fail \n");
        exit(0);
    } else
        printf("listen sucess\n");

    len = sizeof(cli);
    int index = 0;
    int i=0;
    int connections_ids[5];
    while (1){
        connection = (connection_t *) malloc(sizeof(connection_t));
        connection->socket_id = accept(socket_fd, (struct sockaddr*)&cli, &len);

        if (connection->socket_id <= 0)
        {
            free(connection);
        } else{
            printf("client accept\n");
            connections_ids[index] = connection->socket_id;

            for(i=0; i<=index; i++)
            {
                connection->id_conn[i] = connections_ids[i];
            }
            printf("conn id : %i\n",connection->id_conn[index]);
            pthread_create(&thread,0,process,(void *)connection);
            pthread_detach(thread);
//            pthread_join(thread,NULL);
            index++;
        }
    }
}
