#include <raylib.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
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
