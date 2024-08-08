#include "game.h"
#include "net.h"
#include <arpa/inet.h>
#include <raylib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"

char id;

void connect_to_socket() {
  struct sockaddr_in serv_addr;

  // Create socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    exit(-1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    exit(-1);
  }

  // Connect to server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection Failed");
    exit(-1);
  }
}

void player_update(Player *players, int id) {
  connect_to_socket();
  // send player to server
  send(sock, &players[id], sizeof(Player), 0);

  // get player list back
  close(sock);
}

int get_id() {
  connect_to_socket();
  send(sock, "C", 1, 0);

  int id = 0;
  int valread = read(sock, &id, 4);

  close(sock);
  return id;
}

void disconnect() {
  connect_to_socket();
  char msg[2];
  msg[0] = 'D';
  msg[1] = id;

  send(sock, msg, 2, 0);

  close(sock);
}

void client_sigint(int sig) {
  disconnect();
  signal(SIGINT, handle_sigint);
  printf("disconnected\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, client_sigint);

  Player players[MAXPLAYERS];

  id = get_id();
  printf("Got ID: %i\n", id);

  const int screenWidth = 800;
  const int screenHeight = 450;

  char title[20];
  sprintf(title, "balls.io #%i", id);

  InitWindow(screenWidth, screenHeight, title);

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

    players[id].x = circlePosition.x;
    players[id].y = circlePosition.y;
    players[id].id = id;

    /* player_update(players, 0); */
  }

  CloseWindow();
  disconnect();
  return 0;
}
