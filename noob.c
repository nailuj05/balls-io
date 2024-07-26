#include "noob.h"

int main(int argc, const char *argv[]) {
  RebuildYourself(argc, argv);

  int run = HasFlag(argc, argv, "run");

  // GAME
  if (HasFlag(argc, argv, "game")) {
    BuildCommand *bc = CreateBuildCommand(128);
    AddCommand(bc, "gcc game/game.c -g -Iinclude -lraylib -lGL -lm -lpthread "
                   "-ldl -lrt -lX11 -o game/game");
    if (run)
      BuildAndRunCommand("./game/game");

    RunCommand(bc);
    FreeCommand(bc);
  }
  // SERVER
  if (HasFlag(argc, argv, "server")) {
    BuildCommand *bc = CreateBuildCommand(128);
    AddCommand(bc, "gcc server/server.c -Iinclude -o server/server");

    if (run)
      BuildAndRunCommand("./server/server &");

    RunCommand(bc);
    FreeCommand(bc);
  }

  return 0;
}
