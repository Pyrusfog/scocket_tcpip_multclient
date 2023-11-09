#ifndef SERVERTCP_THREAD_H
#define SERVERTCP_THREAD_H
typedef struct
{
    int socket_id;
    char buff[3000];
    char name[10];
    char history[3000];
    int id_conn[5];
    char id_room[5];
} thr_arg;
#endif //SERVERTCP_THREAD_H
