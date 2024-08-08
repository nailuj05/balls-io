#include "net.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER 256

typedef struct {
  char id;
  int x;
  int y;
  int r;
} PlayerHandle;

PlayerHandle **players;

void server_start() {
  players = (PlayerHandle **)malloc(MAXPLAYERS * sizeof(PlayerHandle *));
}

int get_free_id() {
  for (size_t i = 0; i < MAXPLAYERS; ++i) {
    if (players[i] == NULL)
      return i;
  }
  return -1;
}

void handle_request(const char *request, int sock) {
  char *response;
  size_t responseSize = 0;

  if (request[0] == 'C') {
    // Generate and assign id
    int id = get_free_id();
    printf("New player: %i\n", id);

    if (id != -1) {
      // Allocate Player Handle
      players[id] = (PlayerHandle *)malloc(sizeof(PlayerHandle));
      players[id]->id = id;

      response = (char *)malloc(sizeof(char));
      response[0] = id;
      responseSize = 1;
    }
  } else if (request[0] == 'D') {
    printf("Player disconnect: %i\n", request[1]);
    // Disconnect player and remove player handle
    char id = request[1];
    free(players[id]);
    players[id] = NULL;
  }

  // Write
  int valwrite = write(sock, response, responseSize);
  if (valwrite < 0) {
    perror("webserver (write)");
    return;
  }
}

int main() {
  char readBuffer[BUFFER];
  int opt = 1;

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket failed");
    return 1;
  }
  printf("socket created\n");

  signal(SIGINT, handle_sigint);

  // Bind
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    close(sock);
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  server_start();

  // client
  struct sockaddr_in client_addr;
  int client_addrlen = sizeof(client_addr);

  if (bind(sock, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
    perror("bind failed");
    close(sock);
    return 1;
  }
  printf("socket bound to address\n");

  // Listen
  if (listen(sock, SOMAXCONN) != 0) {
    perror("listen failed");
    close(sock);
    return 1;
  }
  printf("server listening on %i\n", PORT);

  for (;;) {
    // Accept conn
    int newsock =
        accept(sock, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
    if (newsock < 0) {
      perror("accept failed");
      continue;
    }

    // Read
    int valread = read(newsock, readBuffer, BUFFER);
    if (valread < 0) {
      perror("webserver (read)");
      continue;
    }

    handle_request(readBuffer, newsock);

    close(newsock);
  }

  return 0;
}
