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

int main(int argc, char **argv)
{
  int sockfd;
  char error[MAXLINE + 1];
  struct sockaddr_in servaddr;

  // verifica se foi passado o ip e a porta para o servidor de processamento se conectar
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

  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
  {
    perror("inet_pton");
    exit(1);
  }

  Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  printf("Servidor de Processamento (Local):\n");
  PrintSocketInfo(sockfd, 1);
  printf("Gerenciador de Tarefas:\n");
  PrintSocketInfo(sockfd, 0);

  for (;;) {
    char recv_buf[MAXDATASIZE];
    char send_buf[MAXDATASIZE];

    Recv(sockfd, recv_buf, MAXDATASIZE, 0);
    if (strcmp(recv_buf, "ENCERRAR") == 0) {
      break;
    }
    sleep(5);
    snprintf(send_buf, sizeof(send_buf), "TAREFA_LIMPEZA CONCLUIDA");
    Send(sockfd, send_buf, strlen(send_buf), 0);

    // zera os valores dos buffers
    memset(recv_buf, 0, sizeof(recv_buf));
    memset(send_buf, 0, sizeof(send_buf));
  }

  Close(sockfd);

  exit(0);
}
