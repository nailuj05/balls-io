#include "net.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER 256

void handle_request(const char *request) {}

void extract_content(const char *str, char *output) {
  const char *start = strchr(str, '{');
  const char *end = strchr(str, '}');

  if (start && end && end > start) {
    size_t length = end - start - 1;
    strncpy(output, start + 1, length);
    output[length] = '\0'; // Null-terminate the output string
  } else {
    // Handle cases where curly brackets are not found
    output[0] = '\0';
  }
}

int main() {
  char readBuffer[BUFFER];
  int opt = 1;
  char response[] = "beep boop i am a server :)\r\n";

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
    printf("connection accepted\n");

    // Read
    int valread = read(newsock, readBuffer, BUFFER);
    if (valread < 0) {
      perror("webserver (read)");
      continue;
    }

    char request[BUFFER];
    extract_content(readBuffer, request);
    printf("%s\n", request);

    // Write
    int valwrite = write(newsock, response, strlen(response));
    if (valwrite < 0) {
      perror("webserver (write)");
      continue;
    }
    close(newsock);
  }

  return 0;
}
