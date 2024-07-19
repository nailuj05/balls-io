#include "net.h"
#include <arpa/inet.h>
#include <raylib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"

int main(int argc, char *argv[]) {
  sock = 0;
  struct sockaddr_in serv_addr;
  char *message = "{Hello, server!}";
  char buffer[1024] = {0};

  // Create socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    return -1;
  }

  signal(SIGINT, handle_sigint);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return -1;
  }

  // Connect to server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection Failed");
    return -1;
  }

  // Send message to server
  send(sock, message, strlen(message), 0);
  printf("Message sent: %s\n", message);

  // Read response from server
  int valread = read(sock, buffer, 1024);
  printf("Response from server: %s\n", buffer);

  return 0;

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "balls.io");

  Vector2 circlePosition = {screenWidth / 2.0f, screenHeight / 2.0f};
  float circleRadius = 20.0f;
  float speed = 200.0f;

  Camera2D camera = {0};
  camera.target = circlePosition;
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_W))
      circlePosition.y -= speed * GetFrameTime();
    if (IsKeyDown(KEY_S))
      circlePosition.y += speed * GetFrameTime();
    if (IsKeyDown(KEY_A))
      circlePosition.x -= speed * GetFrameTime();
    if (IsKeyDown(KEY_D))
      circlePosition.x += speed * GetFrameTime();

    camera.target = circlePosition;

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);

    // Draw Grid
    for (int i = -10; i < 10; ++i) {
      DrawLine(100 * i, 1000, 100 * i, -1000, DARKGRAY);
      DrawLine(1000, 100 * i, -1000, 100 * i, DARKGRAY);
    }

    DrawCircleV(circlePosition, circleRadius, MAROON);
    EndMode2D();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
