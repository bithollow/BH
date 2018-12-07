#ifndef BH_SOCKET_H_
#define BH_SOCKET_H_

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <unistd.h>

class BH_Socket {
public:
    BH_Socket();
    ~BH_Socket();

    int recvFrom(void *buf, int length);
    void init();

private:
    int socket_fd;
};

#endif
