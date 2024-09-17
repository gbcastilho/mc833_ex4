#include "net_api.h"

void Close(int fd)
{
  if (close(fd) == -1)
  {
    perror("close");
    exit(1);
  }
}

int Socket(int family, int type, int flags)
{
  int sockfd;

  if ((sockfd = socket(family, type, flags)) < 0)
  {
    perror("socket");
    exit(1);
  }

  return (sockfd);
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  if (bind(sockfd, addr, addrlen) == -1)
  {
    perror("bind");
    exit(1);
  }
}

void Listen(int sockfd, int backlog)
{
  if (listen(sockfd, backlog) == -1)
  {
    perror("listen");
    exit(1);
  }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
  int connfd;

  if ((connfd = accept(sockfd, addr, addrlen)) == -1)
  {
    perror("accept");
    exit(1);
  }

  return (connfd);
}

void Inet_pton(int family, const char *src, void *dst)
{
  if (inet_pton(family, src, dst) <= 0)
  {
    perror("inet_pton");
    exit(1);
  }
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  if (connect(sockfd, addr, addrlen) < 0)
  {
    perror("connect");
    exit(1);
  }
}

pid_t Fork(void)
{
  pid_t pid;

  if ((pid = fork()) == -1)
  {
    perror("fork");
    exit(1);
  }

  return (pid);
}

void PrintSocketInfo(int sockfd, int local) {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  if (local == 1) {
    if (getsockname(sockfd, (struct sockaddr *)&addr, &addrlen) == -1) {
        perror("getsockname");
        exit(1);
    }
  }
  else {
    if (getpeername(sockfd, (struct sockaddr *)&addr, &addrlen) == -1) {
        perror("getsockname");
        exit(1);
    }
  }

  char ipstr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr.sin_addr), ipstr, sizeof(ipstr));
  
  printf("Endereço IP: %s\n", ipstr);
  printf("Porta: %d\n", ntohs(addr.sin_port));
}