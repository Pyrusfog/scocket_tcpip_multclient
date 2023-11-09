#ifndef SERVERTCP_CONNECTION_H
#define SERVERTCP_CONNECTION_H
#include <sys/socket.h>
typedef struct
{
    int socket_id;
    struct sockaddr address;
    int addr_len;
    int id_conn[5];
} connection_t;
#endif //SERVERTCP_CONNECTION_H
