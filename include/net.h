#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 6969
#define MAXPLAYERS 16

int sock = 0;

void handle_sigint(int sig) {
  printf("Caught signal %d\n", sig);
  if (sock != 0) {
    close(sock);
    printf("Socket closed\n");
  }
  exit(0);
}

#endif
