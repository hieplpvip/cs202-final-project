#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>

namespace Constants {
  const char* TITLE = "Crossing Road";
  const float TITLE_DURATION = 3.0f;

  const int NUMBER_OF_LEVELS = 10;

  std::vector<std::string> MENU_ITEMS = {"New Game", "Load Game", "Settings", "Exit"};
  const std::tuple<float, float, int> LEVELS_CONFIG[NUMBER_OF_LEVELS] = {
      // timeBetweenObstacles, obstacleSpeed, numberOfLanes
      {1.50f, 100.0f, 5},
      {1.40f, 100.0f, 5},
      {1.30f, 110.0f, 6},
      {1.30f, 110.0f, 6},
      {1.20f, 120.0f, 7},
      {1.20f, 120.0f, 7},
      {1.10f, 130.0f, 8},
      {1.10f, 130.0f, 8},
      {1.00f, 140.0f, 9},
      {1.00f, 140.0f, 10},
  };
};

#endif
