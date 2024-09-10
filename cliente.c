#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "net_api.h"

#define MAXLINE 4096

int main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  char error[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    perror(error);
    exit(1);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  Inet_pton(AF_INET, ip, &servaddr.sin_addr);

  getsockname(sockfd, (struct sockaddr *)&servaddr, (socklen_t *)sizeof(servaddr));

  Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  while ((n = read(sockfd, recvline, MAXLINE)) > 0)
  {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF)
    {
      perror("fputs error");
      exit(1);
    }
  }

  if (n < 0)
  {
    perror("read error");
    exit(1);
  }

  exit(0);
}
