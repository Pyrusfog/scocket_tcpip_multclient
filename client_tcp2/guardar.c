#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int socket_id;
    char buff[80];
    char name[10];
} thr_arg;

void * p_send(void *ptr){
    thr_arg *arg_thread;
    arg_thread = (thr_arg *)ptr;
    strcpy(arg_thread->name,"Ed");
    fgets(arg_thread->buff,sizeof(arg_thread->buff),stdin);
    send(arg_thread->socket_id, arg_thread->name, sizeof(arg_thread->name), 0);
    send(arg_thread->socket_id, arg_thread->buff, sizeof(arg_thread->buff), 0);
    bzero(arg_thread->buff,sizeof (arg_thread->buff));
    bzero(arg_thread->name,sizeof (arg_thread->name));
}


void * p_recv(void *ptr){
    thr_arg *arg_thread;
    if (!ptr) pthread_exit(0);
    arg_thread = (thr_arg *)ptr;
    bzero(arg_thread->buff,sizeof (arg_thread->buff));
    bzero(arg_thread->name,sizeof (arg_thread->name));
    recv(arg_thread->socket_id,arg_thread->name,sizeof (arg_thread->name),0);
    recv(arg_thread->socket_id,arg_thread->buff,sizeof (arg_thread->buff),0);
    printf("%s: %s",arg_thread->name,arg_thread->buff);
//    strcat(arg_thread->name,arg_thread->buff);
}

void chat_conn(int socket_id){
    char buff[80];
    char name[10] = "Ed";
    thr_arg *arg_thread;
    pthread_t thread;
//    args.name = "ed";
    int n;
    // infinite loop for chat
    arg_thread = (thr_arg *) malloc(sizeof(thr_arg));
    arg_thread->socket_id = socket_id;
    for (;;) {
        bzero(buff, 80);
        n = 0;
        printf("%s: ",name);
//        strcpy(arg_thread->name,name);
        fgets(buff,sizeof(buff),stdin);
        send(socket_id,name,sizeof(name),0);
        send(socket_id, buff, sizeof(buff),0);
//        pthread_create(&thread,0,p_send,(void *)arg_thread);
//        pthread_create(&thread,0,p_recv,(void *)arg_thread);


        bzero(buff, 80);
//        recv(socket_id,buff,sizeof buff,0);
//        printf("From Server : %s", buff);
        if(strncmp("exit",buff,4) == 0){
            printf("Server Exit...\n");
            break;
        }
//        pthread_join(thread, NULL);
    }
}

int main() {
    int socket_id,connection_id,len;
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
    chat_conn(socket_id);

    close(socket_id);

    return 0;
}
//
// Created by franciscosilva on 31/10/23.
//
