#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <inttypes.h>

#define MAXDATASIZE 100
#define MAXLINE 4096

void Close(int fd);

int Socket(int family, int type, int flags);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Inet_pton(int family, const char *src, void *dst);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

pid_t Fork(void);

void PrintSocketInfo(int sockfd, int local);