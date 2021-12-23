#include <iostream>
#include <limits>
#include "Game.h"
#include "Logging.h"

int main() {
  Logging::setLevel(2);
  Logging::debug("[main] called\n");

  Game game;
  if (game.Construct(250, 360, 2, 2, false, true)) {
    if (!Game::loadData()) {
      Logging::error("[main] Failed to load game data\n");
      return 1;
    }

    game.loading();
    game.Start();

    Game::unloadData();
  }

  Logging::info("Press Enter to exit...");
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}
