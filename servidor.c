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
#include <inttypes.h>
#include "net_api.h"

#define LISTENQ 10

int main(int argc, char **argv)
{
  // TODO: check entry arguments
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);

  pid_t pid;
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  char buf[MAXDATASIZE], recvline[MAXLINE + 1];

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  for (;;)
  {
    connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);

    if ((pid = Fork()) == 0)
    {
      Close(listenfd);
      // TODO; implementar a lógica do servidor
      snprintf(buf, sizeof(buf), "TAREFA: LIMPEZA\n");
      write(connfd, buf, strlen(buf));

      sleep(5);
      Close(connfd);

    
      while ((n = read(connfd, recvline, MAXLINE)) > 0)
      {
        printf("%d\n", n);
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
        {
          perror("fputs error");
          exit(1);
        }
      }

      exit(0);
    }

    Close(connfd);
  }
  return (0);
}
