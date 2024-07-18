#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6969
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
  char response[] = "HTTP/1.0 200 OK\r\n"
                    "Server: webserver-c\r\n"
                    "Content-type: text/html\r\n\r\n"
                    "beep boop i am a server :)\r\n";

  // Create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket failed");
    return 1;
  }
  printf("socket created\n");

  // Bind
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // client
  struct sockaddr_in client_addr;
  int client_addrlen = sizeof(client_addr);

  if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
    perror("bind failed");
    return 1;
  }
  printf("socket bound to address\n");

  // Listen
  if (listen(sockfd, SOMAXCONN) != 0) {
    perror("listen failed");
    return 1;
  }
  printf("server listening on %i\n", PORT);

  for (;;) {
    // Accept conn
    int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                           (socklen_t *)&host_addrlen);
    if (newsockfd < 0) {
      perror("accept failed");
      continue;
    }
    printf("connection accepted\n");

    // Read
    int valread = read(newsockfd, readBuffer, BUFFER);
    if (valread < 0) {
      perror("webserver (read)");
      continue;
    }

    char request[BUFFER];
    extract_content(readBuffer, request);
    printf("%s\n", request);

    // Write
    int valwrite = write(newsockfd, response, strlen(response));
    if (valwrite < 0) {
      perror("webserver (write)");
      continue;
    }
    close(newsockfd);
  }

  return 0;
}
