#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <unistd.h>
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include "bhsocket.h"

#define SERVER_PORT    16666

BH_Socket::BH_Socket()
  : socket_fd(0)
{
    init();
}

BH_Socket::~BH_Socket()
{
    if (socket_fd > 0) {
        close(socket_fd);
    }
}

int BH_Socket::recvFrom(void *buf, int length)
{
    struct sockaddr_in si_other;
    int addr_len = sizeof(struct sockaddr_in);
    return recvfrom(socket_fd, buf, length, 0, (struct sockaddr *)&si_other, (socklen_t*)&addr_len);
}

void BH_Socket::init()
{
    struct sockaddr_in si_me;
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ==-1)
    {
        printf("socket error\n");
    }

    bzero(&si_me, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(SERVER_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
    {
        printf("bind error\n");
    }
}
