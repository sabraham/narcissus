#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const int BACKLOG = 1024;

int socket_open_listen (short port) {
  int s;
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;
  
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '\0', sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  
  if (bind(s, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in)) < 0)
    return -2;

  if (listen(s, BACKLOG) < 0)
    return -3;

  return s;
}

int main (int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./echo port-number");
    return -1;
  }
  int port = atoi(argv[1]);
  int s = socket_open_listen(port);
  int c, clen;
  while (1) {
    c = accept(s, NULL, NULL);
    char buff[16];
    while ((clen = read(c, &buff, sizeof(buff))) > 0)
      write(c, &buff, clen);
    close(c);
  }
  exit(0);
}
