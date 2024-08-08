#include "noob.h"

int main(int argc, const char *argv[]) {
  RebuildYourself(argc, argv);

  int run = HasFlag(argc, argv, "run");

  // GAME
  if (HasFlag(argc, argv, "game") || HasFlag(argc, argv, "client")) {
    BuildCommand *bc = CreateBuildCommand(128);
    AddCommand(bc, "rm -rf game/game && gcc game/game.c -g -Iinclude -lraylib "
                   "-lGL -lm -lpthread "
                   "-ldl -lrt -lX11 -o game/game");

    RunCommand(bc);
    FreeCommand(bc);

    if (run)
      BuildAndRunCommand("./game/game");
  }
  // SERVER
  if (HasFlag(argc, argv, "server")) {
    BuildCommand *bc = CreateBuildCommand(128);
    AddCommand(bc, "rm -rf server/server && gcc server/server.c -Iinclude -o "
                   "server/server");

    RunCommand(bc);
    FreeCommand(bc);

    if (run)
      BuildAndRunCommand("./server/server");
  }

  return 0;
}
