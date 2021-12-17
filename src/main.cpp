#include <iostream>
#include <limits>
#include "Game.h"
#include "Logging.h"

int main() {
  Logging::setLevel(2);
  Logging::debug("[main] called\n");

  Game game;
  if (game.Construct(250, 360, 2, 2, false, true)) {
    game.loading();
    game.Start();
  }

  Logging::info("Press Enter to exit...");
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}
