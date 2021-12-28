#include "Game.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include "Constants.h"
#include "Logging.h"

olc::PixelGameEngine* pge = nullptr;

#define delete_ptr(ptr) \
  if (ptr != nullptr) { \
    delete ptr;         \
    ptr = nullptr;      \
  }

Game::Game() {
  pge = this;
  sAppName = "Crossing Road";
}

void Game::gotoxy(int x, int y) {
  COORD d;
  d.X = x;
  d.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), d);
}

void Game::load() {
  float progress = 0.0;

  while (progress < 1.0) {
    int barWidth = 70;
    int pos = (int)(barWidth * progress);
    gotoxy(25, 16);

    Sleep(50);

    std::cout << "[";
    for (int i = 0; i < barWidth; i++) {
      if (i < pos) {
        std::cout << "=";
      } else if (i == pos) {
        std::cout << ">";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "]" << int(progress * 100.0) << " %\r";
    std::cout.flush();

    progress += 0.01f;
  }
  std::cout << std::endl;
}

void Game::loading() {
  PlaySound(TEXT("MenuSound.wav"), NULL, SND_LOOP | SND_ASYNC);
  gotoxy(55, 14);
  std::cout << "Loading..." << std::endl;
  // gotoxy(10, 16);
  load();
}

bool Game::OnUserCreate() {
  Logging::debug("[Game::OnUserCreate] Initializing game engine\n");

  olc::SOUND::InitialiseAudio();

  if (!Bird::loadData()) return false;
  if (!Car::loadData()) return false;
  if (!Coin::loadData()) return false;
  if (!Elephant::loadData()) return false;
  if (!Lane::loadData()) return false;
  if (!Level::loadData()) return false;
  if (!TrafficLight::loadData()) return false;
  if (!Truck::loadData()) return false;

  gameState = GAME_STATE_TITLE;
  timeAccumulator = 0;
  selectedMenuItem = 0;
  selectedPauseItem = 0;
  selectedSettingItem = 0;
  currentLevel = 10;
  coinEaten = 0;

  player = new Player();
  trafficLight = new TrafficLight();
  level = nullptr;

  return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
  Clear(olc::BLANK);
  timeAccumulator += fElapsedTime;

  switch (gameState) {
    case GAME_STATE_TITLE: {
      float titleScale = 1.5f;
      olc::vi2d titleSize = (olc::vf2d)(GetTextSize(Constants::TITLE)) * titleScale;
      olc::vi2d titlePos = ScreenSize() / 2 - titleSize / 2;
      DrawStringDecal(titlePos, Constants::TITLE, olc::RED, {titleScale, titleScale});

      if (timeAccumulator > Constants::TITLE_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_MENU: {
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedMenuItem == 0) {
          // New Game
          Logging::info("New game!\n");
          newGame();
          return true;
        } else if (selectedMenuItem == 1) {
          // Load Game
          gameState = GAME_STATE_LOADGAME;
          timeAccumulator = 0;
        } else if (selectedMenuItem == 2) {
          // Settings
          selectedSettingItem = 0;
          gameState = GAME_STATE_SETTINGS;
          timeAccumulator = 0;
        } else {
          // Exit
          Logging::info("Exit!\n");
          return false;
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedMenuItem;
        if (selectedMenuItem >= Constants::MENU_ITEMS.size()) {
          selectedMenuItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedMenuItem;
        if (selectedMenuItem < 0) {
          selectedMenuItem = (int)Constants::MENU_ITEMS.size() - 1;
        }
      }

      int height = GetTextSize(Constants::MENU_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)Constants::MENU_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::MENU_ITEMS.size(); ++i) {
        float textScale = (i == selectedMenuItem) ? 1.5f : 1.2f;
        olc::Pixel textColor = (i == selectedMenuItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(GetTextSize(Constants::MENU_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        DrawStringDecal(textPos, Constants::MENU_ITEMS[i], textColor, {textScale, textScale});
      }

      break;
    }
    case GAME_STATE_LOADING: {
      // TODO: Initialize level here
      break;
    }
    case GAME_STATE_PLAY: {
      if (GetKey(olc::ESCAPE).bPressed) {
        gameState = GAME_STATE_PAUSE;
        timeAccumulator = 0;
        return true;
      }

      int dx = 0, dy = 0;
      if (GetKey(olc::LEFT).bHeld) --dx;
      if (GetKey(olc::RIGHT).bHeld) ++dx;
      if (GetKey(olc::UP).bHeld) --dy;
      if (GetKey(olc::DOWN).bHeld) ++dy;
      if (dx == -1) {
        player->moveLeft(fElapsedTime);
      } else if (dx == 1) {
        player->moveRight(fElapsedTime);
      }
      if (dy == -1) {
        player->moveUp(fElapsedTime);
      } else if (dy == 1) {
        player->moveDown(fElapsedTime);
      }

      trafficLight->update(fElapsedTime);
      if (trafficLight->getColor() != RED) {
        level->update(fElapsedTime);
      }

      level->draw();
      trafficLight->draw();
      player->draw();

      DrawString(10, 10, "Level: " + std::to_string(currentLevel), olc::RED);
      DrawString(10, 20, "Score: " + std::to_string(coinEaten * 10), olc::RED);

      if (level->checkCollision()) {
        Logging::info("Hit obstacle! Game lost\n");
        gameState = GAME_STATE_GAMEOVER;
        timeAccumulator = 0;
        return true;
      }

      // Check if player earns any coins
      level->checkCoin(coinEaten);

      if (level->isComplete()) {
        if (currentLevel < Constants::NUMBER_OF_LEVELS) {
          // Go to next level
          Logging::info("Level %d completed. Go to next level!\n", currentLevel);
          nextLevel();
        } else {
          // Win game
          Logging::info("Game won!\n");
          gameState = GAME_STATE_WIN;
          timeAccumulator = 0;
        }
        return true;
      }

      break;
    }
    case GAME_STATE_PAUSE: {
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedPauseItem == 0) {
          // Continue
          gameState = GAME_STATE_PLAY;
        } else if (selectedPauseItem == 1) {
          // Back to menu
          selectedLoadItem = 0;
          gameState = GAME_SAVE;
        } else {
          gameState = GAME_STATE_MENU;
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedPauseItem;
        if (selectedPauseItem >= Constants::PAUSE_ITEMS.size()) {
          selectedPauseItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedPauseItem;
        if (selectedPauseItem < 0) {
          selectedPauseItem = (int)Constants::PAUSE_ITEMS.size() - 1;
        }
      }
      // std::cout << selectedSettingItem << endl;
      int height = GetTextSize(Constants::PAUSE_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)Constants::PAUSE_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::PAUSE_ITEMS.size(); ++i) {
        float textScale = (i == selectedPauseItem) ? 1.5f : 1.2f;
        olc::Pixel textColor = (i == selectedPauseItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(GetTextSize(Constants::PAUSE_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        DrawStringDecal(textPos, Constants::PAUSE_ITEMS[i], textColor, {textScale, textScale});
      }
      break;
    }
    case GAME_STATE_WIN: {
      float winScale = 2.5f;
      olc::vi2d winSize = (olc::vf2d)(GetTextSize(Constants::WIN)) * winScale;
      olc::vi2d winPos = ScreenSize() / 2 - winSize / 2;
      winPos.y -= 10;
      DrawStringDecal(winPos, Constants::WIN, olc::RED, {winScale, winScale});

      std::string score = "Your score: " + std::to_string(coinEaten * 10);
      olc::vi2d scorePos = {ScreenWidth() / 2 - GetTextSize(score).x / 2, winPos.y + winSize.y + 10};
      DrawString(scorePos, score, olc::RED);

      if (timeAccumulator > Constants::WIN_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_GAMEOVER: {
      float loseScale = 2.5f;
      olc::vi2d loseSize = (olc::vf2d)(GetTextSize(Constants::LOSE)) * loseScale;
      olc::vi2d losePos = ScreenSize() / 2 - loseSize / 2;
      DrawStringDecal(losePos, Constants::LOSE, olc::RED, {loseScale, loseScale});

      if (timeAccumulator > Constants::LOSE_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_SAVE: {
      std::ifstream f("src/SaveGame/checkpoint.txt");
      std::vector<std::string> LOAD_ITEMS;
      std::string input;
      while (std::getline(f, input)) {
        LOAD_ITEMS.push_back(input);
      }
      LOAD_ITEMS.push_back("Back To Pause");
      f.close();
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedLoadItem == 0) {
          // Save 1
          LOAD_ITEMS[0] = "Save_1";
        } else if (selectedLoadItem == 1) {
          // Save 2
          LOAD_ITEMS[1] = "Save_2";
        } else if (selectedLoadItem == 2) {
          // Save 3
          LOAD_ITEMS[2] = "Save_3";
        }
        if (selectedLoadItem == 3) {
          gameState = GAME_STATE_PAUSE;
        } else {
          std::ofstream fo("src/SaveGame/checkpoint.txt");
          for (int i = 0; i < 3; i++) {
            fo << LOAD_ITEMS[i] << std::endl;
          }
          fo.close();
          std::ofstream fout("src/SaveGame/" + LOAD_ITEMS[selectedLoadItem] + ".dat", std::ios::binary);
          fout.write((char*)&currentLevel, sizeof(currentLevel));
          int score = coinEaten * 10;
          fout.write((char*)&score, sizeof(score));
          auto [X, Y] = player->getPosition();
          fout.write((char*)&X, sizeof(X));
          fout.write((char*)&Y, sizeof(Y));
          fout.close();
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedLoadItem;
        if (selectedLoadItem >= LOAD_ITEMS.size()) {
          selectedLoadItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedLoadItem;
        if (selectedLoadItem < 0) {
          selectedLoadItem = (int)LOAD_ITEMS.size() - 1;
        }
      }
      // std::cout << selectedSettingItem << endl;
      int height = GetTextSize(LOAD_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)LOAD_ITEMS.size()) / 2;
      for (int i = 0; i < (int)LOAD_ITEMS.size(); ++i) {
        float textScale = (i == selectedLoadItem) ? 1.5f : 1.2f;
        olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(GetTextSize(LOAD_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        DrawStringDecal(textPos, LOAD_ITEMS[i], textColor, {textScale, textScale});
      }

      if (timeAccumulator > 101) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_LOADGAME: {
      std::ifstream f("src/SaveGame/checkpoint.txt");
      std::vector<std::string> LOAD_ITEMS;
      std::string input;
      while (std::getline(f, input)) {
        // std::cout << input << endl;
        LOAD_ITEMS.push_back(input);
      }
      LOAD_ITEMS.push_back("Back To Menu");
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedLoadItem == 3) {
          gameState = GAME_STATE_MENU;
        } else {
          std::ifstream fin("src/SaveGame/" + LOAD_ITEMS[selectedLoadItem] + ".dat", std::ios::out | std::ios::binary);
          fin.read((char*)&currentLevel, sizeof(currentLevel));
          fin.read((char*)&coinEaten, sizeof(coinEaten));
          float X, Y;
          fin.read((char*)&X, sizeof(X));
          fin.read((char*)&Y, sizeof(Y));
          player->setPosition({X, Y});
          gameState = GAME_STATE_PLAY;
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedLoadItem;
        if (selectedLoadItem >= LOAD_ITEMS.size()) {
          selectedLoadItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedLoadItem;
        if (selectedLoadItem < 0) {
          selectedLoadItem = (int)LOAD_ITEMS.size() - 1;
        }
      }
      // std::cout << selectedSettingItem << endl;
      int height = GetTextSize(LOAD_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)LOAD_ITEMS.size()) / 2;
      for (int i = 0; i < (int)LOAD_ITEMS.size(); ++i) {
        float textScale = (i == selectedLoadItem) ? 1.5f : 1.2f;
        olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(GetTextSize(LOAD_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        DrawStringDecal(textPos, LOAD_ITEMS[i], textColor, {textScale, textScale});
      }

      if (timeAccumulator > 101) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_SETTINGS: {
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedSettingItem == 0) {
          // Change Level
          currentLevel += 1;
          if (currentLevel == 11)
            currentLevel = 1;
        } else if (selectedSettingItem == 1) {
          // Change Sound
          if (sound == 0)
            sound = 1;
          else
            sound = 0;
          if (sound == 0)
            PlaySound(NULL, NULL, NULL);
          else
            PlaySound(TEXT("MenuSound.wav"), NULL, SND_LOOP | SND_ASYNC);

        } else
          timeAccumulator = 101;

      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedSettingItem;
        if (selectedSettingItem >= Constants::SETTING_ITEMS.size()) {
          selectedSettingItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedSettingItem;
        if (selectedSettingItem < 0) {
          selectedSettingItem = (int)Constants::SETTING_ITEMS.size() - 1;
        }
      }
      // std::cout << selectedSettingItem << endl;
      int height = GetTextSize(Constants::SETTING_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)Constants::SETTING_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::SETTING_ITEMS.size(); ++i) {
        float textScale = (i == selectedSettingItem) ? 1.5f : 1.2f;
        olc::Pixel textColor = (i == selectedSettingItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(GetTextSize(Constants::SETTING_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        if (i == 1) {
          if (sound == 0)
            DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + "OFF", textColor, {textScale, textScale});
          else
            DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + "ON", textColor, {textScale, textScale});
        } else if (i == 0) {
          DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + std::to_string(currentLevel), textColor, {textScale, textScale});
        }
        DrawStringDecal(textPos, Constants::SETTING_ITEMS[i], textColor, {textScale, textScale});
      }
      // DrawString(0, 0, "Work in progress", olc::RED);

      if (timeAccumulator > 100) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
  };

  return true;
}

bool Game::OnUserDestroy() {
  Logging::debug("[Game::OnUserDestroy] Destroying objects\n");

  Bird::unloadData();
  Car::unloadData();
  Coin::unloadData();
  Elephant::unloadData();
  Lane::unloadData();
  Level::unloadData();
  TrafficLight::unloadData();
  Truck::unloadData();

  olc::SOUND::DestroyAudio();

  delete_ptr(player);

  return true;
}

void Game::newGame() {
  gameState = GAME_STATE_PLAY;
  coinEaten = 0;
  timeAccumulator = 0;
  generateLevel();
}

void Game::nextLevel() {
  assert(currentLevel < Constants::NUMBER_OF_LEVELS);
  ++currentLevel;
  generateLevel();
}

void Game::generateLevel() {
  Logging::debug("[Game::generateLevel] Generate level %d\n", currentLevel);
  delete_ptr(level);
  auto [timeBetweenObstacles, obstacleSpeed, numberOfLanes] = Constants::LEVELS_CONFIG[currentLevel - 1];
  level = new Level(timeBetweenObstacles, obstacleSpeed, numberOfLanes, player, currentLevel);
  trafficLight->reset();
}
