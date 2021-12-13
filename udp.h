#ifndef __UDP_h__
#define __UDP_h__

//
// includes
// 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netinet/tcp.h>
#include <netinet/in.h>

typedef struct message{
    int call;
    int inum;
	int pinum;
    char *buffer;
	char *name;
    int block;
    int file_or_dir;
}message;

int UDP_Open(int port);
int UDP_Close(int fd);

int UDP_Read(int fd, struct sockaddr_in *addr, struct message *msg, int n);
int UDP_Write(int fd, struct sockaddr_in *addr, struct message *msg, int n);

int UDP_FillSockAddr(struct sockaddr_in *addr, char *hostName, int port);

#endif // __UDP_h__
