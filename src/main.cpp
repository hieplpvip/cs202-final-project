#include <iostream>
#include <limits>
#include "Game.h"

int main() {
  Game game;
  if (game.Construct(256, 256, 2, 2)) {
    game.Start();
  }

  //std::cout << "Press Enter to exit...";
  //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}
