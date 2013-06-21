#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>

const int port = 5000;

int socket_open_and_connect (char *hostname, int port) {
  int s;
  struct hostent *h;
  
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Could not open socket.\n");
    return -1;
  }
  
  if (!(h = gethostbyname(hostname))) {
    printf("Hostname lookup failed.\n");
    return -2;
  }
  
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '\0', sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr = *((struct in_addr *) h->h_addr_list[0]);
  
  if (connect(s, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in)) < 0) {
    printf("Could not connect to %s at %s:%hd\n",
           h->h_name, inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    return -3;
  }

  return s;
}

int main(int argc, char** argv){
  if (argc != 3) {
    printf("Usage: ./narcissus hostname port-number");
    return -1;
  }
  int s;
  char* hostname = argv[1];
  int port = atoi(argv[2]);

  if ((s = socket_open_and_connect(hostname, port)) < 0) {
    return -2;
  }

  int buff_size = 10;
  char buff[buff_size];
  int c, slen;
  while (1) {
    while ((c = getchar()) != EOF){
      write(s, &c, 1);
      if (c == '\n')
        break;
    }
    bool reading = true;
    while (reading && ((slen = read(s, &buff, buff_size)) > 0)) {
      write(STDOUT_FILENO, buff, slen);
      int i;
      for (i = 0; i < slen; ++i) {
        if (buff[i] == '\n') {
          reading = false;
          break;
        }
      }
    }
  }
  exit(0);
}
