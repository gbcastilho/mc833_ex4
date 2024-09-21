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
#include <fcntl.h>
#include <sys/stat.h>
#include "net_api.h"
#include "server.h"

#define LISTENQ 10

int main(int argc, char **argv)
{
  // verifica se a porta do servidor foi passada como parametro
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);

  int listenfd, connfd;
  struct sockaddr_in servaddr;

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

    pid_t pid;
    if ((pid = Fork()) == 0)
    {
      Close(listenfd);
      // abre o arquivo que sera registrados os logs do gerenciados
      int fd = open("server_logs.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
      if (fd == -1) {
        perror("open");
        exit(1);
      }

      // obtem o IP e a porta do servidor de processamento conectado
      struct sockaddr_in addr;
      socklen_t addrlen = sizeof(addr);
      Getpeername(connfd, (struct sockaddr *)&addr, &addrlen);
      char ipstr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(addr.sin_addr), ipstr, sizeof(ipstr));

      SendReq(connfd, fd, "TAREFA: LIMPEZA", ipstr, ntohs(addr.sin_port));
      ReceiveRes(connfd, fd, ipstr, ntohs(addr.sin_port));

      // descomentar as linhas caso queira mandar mais de uma tarefa para o servidor de processamento
      // SendReq(connfd, fd, "TAREFA: LIMPEZA", ipstr, ntohs(addr.sin_port));
      // ReceiveRes(connfd, fd, ipstr, ntohs(addr.sin_port));

      SendReq(connfd, fd, "ENCERRAR", ipstr, ntohs(addr.sin_port));

      Close(fd);

      Close(connfd);

      exit(0);
    }

    Close(connfd);
  }
  return (0);
}
