#include "Game.h"
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include "Constants.h"
#include "Logging.h"
#include "Utility.h"

olc::PixelGameEngine* pge = nullptr;

int Game::sndIntro = -1;
int Game::sndInGame = -1;
olc::Font* Game::erasFont = nullptr;

#define delete_ptr(ptr) \
  if (ptr != nullptr) { \
    delete ptr;         \
    ptr = nullptr;      \
  }

Game::Game() {
  pge = this;
  sAppName = "Crossing Road";
}

bool Game::OnUserCreate() {
  Logging::debug("[Game::OnUserCreate] Initializing game engine\n");

  olc::SOUND::InitialiseAudio();

  sndIntro = olc::SOUND::LoadAudioSample("assets/sound/NFL Theme Song (HQ).wav");
  sndInGame = olc::SOUND::LoadAudioSample("assets/sound/MC Hammer - U Can't Touch This.wav");
  erasFont = new olc::Font("assets/fonts/eras.png");

  if (sndIntro == -1 || sndInGame == -1 ||
      !Bird::loadData() ||
      !Car::loadData() ||
      !Coin::loadData() ||
      !Elephant::loadData() ||
      !Lane::loadData() ||
      !Level::loadData() ||
      !Player::loadData() ||
      !TrafficLight::loadData() ||
      !Truck::loadData()) {
    Logging::error("[Game::OnUserCreate] Failed to load data\n");
    return false;
  }

  gameState = GAME_STATE_LOADING;
  timeAccumulator = 0;
  selectedMenuItem = 0;
  selectedPauseItem = 0;
  selectedSettingItem = 0;
  difficulty = 1;
  soundEnabled = 1;
  currentSound = -1;
  currentLives = 3;
  currentLevel = 1;
  currentPoints = 0;
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
    case GAME_STATE_LOADING: {
      float loadingScale = 0.1f;
      olc::vi2d loadingSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::LOADING)) * loadingScale;
      olc::vi2d loadingPos = ScreenSize() / 2 - loadingSize / 2;
      erasFont->DrawStringPropDecal(loadingPos, Constants::LOADING, olc::WHITE, {loadingScale, loadingScale});

      olc::vi2d progressBarSize = {150, 10};
      olc::vi2d progressBarPos = ScreenSize() / 2 - progressBarSize / 2;
      progressBarPos.y += loadingSize.y + 5;
      FillRect(progressBarPos, {(int)(150.0f * timeAccumulator / Constants::LOADING_DURATION), 10}, olc::RED);
      DrawRect(progressBarPos, progressBarSize, olc::WHITE);

      if (timeAccumulator > Constants::LOADING_DURATION) {
        gameState = GAME_STATE_TITLE;
        timeAccumulator = 0;
        currentSound = sndIntro;
        olc::SOUND::PlaySample(currentSound, true);
      }

      break;
    }
    case GAME_STATE_TITLE: {
      float titleScale = 0.15f;
      olc::vi2d titleSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::TITLE)) * titleScale;
      olc::vi2d titlePos = ScreenSize() / 2 - titleSize / 2;
      erasFont->DrawStringPropDecal(titlePos, Constants::TITLE, olc::RED, {titleScale, titleScale});

      if (timeAccumulator > Constants::TITLE_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        selectedMenuItem = 0;
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_MENU: {
      currentSound = sndIntro;
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedMenuItem == 0) {
          // New Game
          gameState = GAME_STATE_NEWGAME;
          timeAccumulator = 0;
          if (currentSound != -1) {
            olc::SOUND::StopSample(currentSound);
            currentSound = -1;
          }
        } 
        else if (selectedMenuItem == 1) {
            // Load Game
            selectedLoadItem = 3;
            gameState = GAME_STATE_LOADGAME;
            timeAccumulator = 0;
        }
        else if (selectedMenuItem == 2) {
          // Settings
          selectedSettingItem = 2;
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

      int height = (int)(erasFont->GetTextSizeProp(Constants::MENU_ITEMS[0]).y * 0.12f);
      int offset = (ScreenHeight() - height * (int)Constants::MENU_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::MENU_ITEMS.size(); ++i) {
        float textScale = (i == selectedMenuItem) ? 0.12f : 0.1f;
        olc::Pixel textColor = (i == selectedMenuItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::MENU_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        erasFont->DrawStringPropDecal(textPos, Constants::MENU_ITEMS[i], textColor, {textScale, textScale});
      }

      break;
    }
    case GAME_STATE_NEWGAME: {
      float scale = 0.1f;
      std::string s = "Prepare for Level 1...";
      olc::vi2d size = (olc::vf2d)(erasFont->GetTextSizeProp(s)) * scale;
      olc::vi2d pos = ScreenSize() / 2 - size / 2;
      erasFont->DrawStringPropDecal(pos, s, olc::RED, {scale, scale});

      if (timeAccumulator > 3.0f || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        newGame();
      }

      break;
    }
    case GAME_STATE_NEXTLEVEL: {
      {
        float scale = 0.1f;
        std::string s = "You passed Level " + std::to_string(currentLevel) + "!";
        olc::vi2d size = (olc::vf2d)(erasFont->GetTextSizeProp(s)) * scale;
        olc::vi2d pos = ScreenSize() / 2 - size / 2;
        erasFont->DrawStringPropDecal(pos, s, olc::RED, {scale, scale});
      }
      {
        float scale = 0.1f;
        std::string s = "Prepare for Level " + std::to_string(currentLevel + 1) + "...";
        olc::vi2d size = (olc::vf2d)(erasFont->GetTextSizeProp(s)) * scale;
        olc::vi2d pos = ScreenSize() / 2 - size / 2;
        pos.y += size.y + 5;
        erasFont->DrawStringPropDecal(pos, s, olc::RED, {scale, scale});
      }

      if (timeAccumulator > 3.0f || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        nextLevel();
      }

      break;
    }
    case GAME_STATE_PLAY: {
      if (GetKey(olc::ESCAPE).bPressed) {
        selectedPauseItem = 0;
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

      std::ifstream f("Score/HighScore.dat", std::ios::out | std::ios::binary);
      int currentHigh;
      f.read((char*)&currentHigh, sizeof(currentHigh));
      f.close();
      
      DrawString(10, 5, "Level: " + std::to_string(currentLevel), olc::WHITE);
      DrawString(10, 15, "Lives: " + std::to_string(currentLives), olc::WHITE);
      DrawString(10, 25, "Score: " + std::to_string(currentPoints + coinEaten * 10), olc::WHITE);
      DrawString(10, 35, "High score: " + std::to_string(currentPoints + std::max(coinEaten * 10, currentHigh)), olc::WHITE);

      if (level->checkCollision(soundEnabled)) {
        Logging::info("Hit obstacle! ");
        if (currentLives == 1) {
          Logging::info("Game lost!\n");
          gameState = GAME_STATE_GAMEOVER;
        } else {
          Logging::info("Let's try again!\n");
          gameState = GAME_STATE_HIT;
          --currentLives;
        }
        timeAccumulator = 0;
        if (soundEnabled)
        {
            if (currentSound != -1) {
                olc::SOUND::StopSample(currentSound);
                currentSound = -1;
            }
        }
        return true;
      }

      // Check if player earns any coins
      level->checkCoin(coinEaten);

      if (level->isComplete()) {
        currentPoints += 10 * currentLevel * difficulty;
        if (currentLives < 3) {
          ++currentLives;
        }
        if (currentLevel < Constants::NUMBER_OF_LEVELS) {
          // Go to next level
          gameState = GAME_STATE_NEXTLEVEL;
          timeAccumulator = 0;
          if (soundEnabled)
          {
              if (currentSound != -1) {
                  olc::SOUND::StopSample(currentSound);
                  currentSound = -1;
              }
          }
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
          // Save Game
          selectedLoadItem = 0;
          gameState = GAME_STATE_SAVEGAME;
        } else {
          // Back to menu
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

      int height = (int)(erasFont->GetTextSizeProp(Constants::PAUSE_ITEMS[0]).y * 0.12f);
      int offset = (ScreenHeight() - height * (int)Constants::PAUSE_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::PAUSE_ITEMS.size(); ++i) {
        float textScale = (i == selectedPauseItem) ? 0.12f : 0.1f;
        olc::Pixel textColor = (i == selectedPauseItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::PAUSE_ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        erasFont->DrawStringPropDecal(textPos, Constants::PAUSE_ITEMS[i], textColor, {textScale, textScale});
      }
      break;
    }
    case GAME_STATE_WIN: {
      float winScale = 0.25f;
      olc::vi2d winSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::WIN)) * winScale;
      olc::vi2d winPos = ScreenSize() / 2 - winSize / 2;
      winPos.y -= 10;
      erasFont->DrawStringPropDecal(winPos, Constants::WIN, olc::RED, {winScale, winScale});

      float scoreScale = 0.1f;
      std::string score = "Your score: " + std::to_string(currentPoints + coinEaten * 10);
      olc::vi2d scoreSize = (olc::vf2d)(erasFont->GetTextSizeProp(score)) * scoreScale;
      olc::vi2d scorePos = {ScreenWidth() / 2 - scoreSize.x / 2, winPos.y + winSize.y + 5};
      erasFont->DrawStringPropDecal(scorePos, score, olc::RED, {scoreScale, scoreScale});

      if (timeAccumulator > Constants::WIN_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        selectedMenuItem = 0;
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_HIT: {
      float hitScale = 0.1f;
      olc::vi2d hitSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::HIT)) * hitScale;
      olc::vi2d hitPos = ScreenSize() / 2 - hitSize / 2;
      erasFont->DrawStringPropDecal(hitPos, Constants::HIT, olc::RED, {hitScale, hitScale});

      if (timeAccumulator > Constants::HIT_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        gameState = GAME_STATE_PLAY;
        timeAccumulator = 0;
        generateLevel();
        currentSound = sndInGame;
        if (soundEnabled)
        {
            olc::SOUND::PlaySample(currentSound, true);
        }
      }

      break;
    }
    case GAME_STATE_GAMEOVER: {
      float loseScale = 0.25f;
      olc::vi2d loseSize = (olc::vf2d)(erasFont->GetTextSizeProp(Constants::LOSE)) * loseScale;
      olc::vi2d losePos = ScreenSize() / 2 - loseSize / 2;
      erasFont->DrawStringPropDecal(losePos, Constants::LOSE, olc::RED, {loseScale, loseScale});

      int score = 0;
      std::ifstream f("Score/HighScore.dat", std::ios::out | std::ios::binary);
      f.read((char*)&score, sizeof(score));
      if (score < coinEaten * 10)
          score = coinEaten * 10;

      f.close();

      std::ofstream c("Score/HighScore.dat", std::ios::binary);
      c.write((char*)&score, sizeof(score));
      c.close();

      if (timeAccumulator > Constants::LOSE_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        selectedMenuItem = 0;
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
        currentSound = sndIntro;
        if (soundEnabled) {
            olc::SOUND::PlaySample(currentSound, true);
        }
      }

      break;
    }
    case GAME_STATE_SAVEGAME: {
      std::vector<std::string> ITEMS;
      for (int i = 1; i <= Constants::NUMBER_OF_SAVE_SLOTS; ++i) {
        std::string filename = "SaveGame/save_" + std::to_string(i) + ".dat";
        if (fileExists(filename)) {
          ITEMS.push_back("Slot " + std::to_string(i) + " (saved)");
        } else {
          ITEMS.push_back("Slot " + std::to_string(i) + " (empty)");
        }
      }
      ITEMS.push_back("Back To Menu");

      if (GetKey(olc::ENTER).bPressed) {
        if (selectedLoadItem == (int)ITEMS.size() - 1) {
          gameState = GAME_STATE_PAUSE;
        } else {
          std::string filename = "SaveGame/save_" + std::to_string(selectedLoadItem + 1) + ".dat";
          std::ofstream f(filename, std::ios::binary);
          f.write((char*)&difficulty, sizeof(difficulty));
          f.write((char*)&currentLevel, sizeof(currentLevel));
          f.write((char*)&currentPoints, sizeof(currentPoints));
          f.write((char*)&coinEaten, sizeof(coinEaten));
          auto [X, Y] = player->getPosition();
          f.write((char*)&X, sizeof(X));
          f.write((char*)&Y, sizeof(Y));
          f.close();
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedLoadItem;
        if (selectedLoadItem >= ITEMS.size()) {
          selectedLoadItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedLoadItem;
        if (selectedLoadItem < 0) {
          selectedLoadItem = (int)ITEMS.size() - 1;
        }
      }

      int height = (int)(erasFont->GetTextSizeProp(ITEMS[0]).y * 0.12f);
      int offset = (ScreenHeight() - height * (int)ITEMS.size()) / 2;
      for (int i = 0; i < (int)ITEMS.size(); ++i) {
        float textScale = (i == selectedLoadItem) ? 0.12f : 0.1f;
        olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(erasFont->GetTextSizeProp(ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        erasFont->DrawStringPropDecal(textPos, ITEMS[i], textColor, {textScale, textScale});
      }

      break;
    }
    case GAME_STATE_LOADGAME: {
      std::vector<std::string> ITEMS;
      for (int i = 1; i <= Constants::NUMBER_OF_SAVE_SLOTS; ++i) {
        std::string filename = "SaveGame/save_" + std::to_string(i) + ".dat";
        if (fileExists(filename)) {
          ITEMS.push_back("Slot " + std::to_string(i) + " (saved)");
        } else {
          ITEMS.push_back("Slot " + std::to_string(i) + " (empty)");
        }
      }
      ITEMS.push_back("Back To Menu");

      if (GetKey(olc::ENTER).bPressed) {
        if (selectedLoadItem == (int)ITEMS.size() - 1) {
          gameState = GAME_STATE_MENU;
        } else {
          std::string filename = "SaveGame/save_" + std::to_string(selectedLoadItem + 1) + ".dat";
          if (fileExists(filename)) {
            std::ifstream f(filename, std::ios::out | std::ios::binary);
            f.read((char*)&difficulty, sizeof(difficulty));
            f.read((char*)&currentLevel, sizeof(currentLevel));
            f.read((char*)&currentPoints, sizeof(currentPoints));
            f.read((char*)&coinEaten, sizeof(coinEaten));
            float X, Y;
            f.read((char*)&X, sizeof(X));
            f.read((char*)&Y, sizeof(Y));
            f.close();

            player->setPosition({X, Y});
            gameState = GAME_STATE_PLAY;
          }
        }
      } else if (GetKey(olc::DOWN).bPressed) {
        ++selectedLoadItem;
        if (selectedLoadItem >= ITEMS.size()) {
          selectedLoadItem = 0;
        }
      } else if (GetKey(olc::UP).bPressed) {
        --selectedLoadItem;
        if (selectedLoadItem < 0) {
          selectedLoadItem = (int)ITEMS.size() - 1;
        }
      }

      int height = (int)(erasFont->GetTextSizeProp(ITEMS[0]).y * 0.12f);
      int offset = (ScreenHeight() - height * (int)ITEMS.size()) / 2;
      for (int i = 0; i < (int)ITEMS.size(); ++i) {
        float textScale = (i == selectedLoadItem) ? 0.12f : 0.1f;
        olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(erasFont->GetTextSizeProp(ITEMS[i])) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        erasFont->DrawStringPropDecal(textPos, ITEMS[i], textColor, {textScale, textScale});
      }

      if (timeAccumulator > 100) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_SETTINGS: {
      if (GetKey(olc::ENTER).bPressed) {
        if (selectedSettingItem == 0) {
          // Change Difficulty
          difficulty += 1;
          if (difficulty == 4) {
            difficulty = 1;
          }
        } else if (selectedSettingItem == 1) {
          // Toggle Sound
          soundEnabled = 1 - soundEnabled;
          currentSound = sndIntro;
          if (currentSound != -1) {
            if (soundEnabled) {
              olc::SOUND::PlaySample(currentSound, true);
            } else {
              olc::SOUND::StopSample(currentSound);
            }
          }
        } else {
          timeAccumulator = 101;
        }
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

      int height = (int)(erasFont->GetTextSizeProp(Constants::SETTING_ITEMS[0]).y * 0.12f);
      int offset = (ScreenHeight() - height * (int)Constants::SETTING_ITEMS.size()) / 2;
      for (int i = 0; i < (int)Constants::SETTING_ITEMS.size(); ++i) {
        std::string text = Constants::SETTING_ITEMS[i];
        if (i == 1) {
          text += (soundEnabled) ? "ON" : "OFF";
        } else if (i == 0) {
          text += (difficulty == 1) ? "Easy" : ((difficulty == 2) ? "Medium" : "Hard");
        }

        float textScale = (i == selectedSettingItem) ? 0.12f : 0.1f;
        olc::Pixel textColor = (i == selectedSettingItem) ? olc::YELLOW : olc::WHITE;

        olc::vi2d center = {ScreenWidth() / 2, offset + height * (2 * i + 1) / 2};
        olc::vi2d textSize = (olc::vf2d)(erasFont->GetTextSizeProp(text)) * textScale;
        olc::vi2d textPos = center - textSize / 2;
        erasFont->DrawStringPropDecal(textPos, text, textColor, {textScale, textScale});
      }

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
  Player::unloadData();
  TrafficLight::unloadData();
  Truck::unloadData();

  olc::SOUND::DestroyAudio();

  delete_ptr(player);

  return true;
}

void Game::newGame() {
  Logging::info("New Game!\n");
  gameState = GAME_STATE_PLAY;
  currentPoints = 0;
  coinEaten = 0;
  timeAccumulator = 0;
  currentLives = 3;
  currentLevel = 1;
  generateLevel();
  currentSound = sndInGame;
  if (soundEnabled)
  {
      olc::SOUND::PlaySample(currentSound, true);
  }
}

void Game::nextLevel() {
  assert(currentLevel < Constants::NUMBER_OF_LEVELS);
  Logging::info("Level %d completed. Go to next level!\n", currentLevel);
  gameState = GAME_STATE_PLAY;
  timeAccumulator = 0;
  ++currentLevel;
  generateLevel();
  currentSound = sndInGame;
  if (soundEnabled)
  {
      olc::SOUND::PlaySample(currentSound, true);
  }
}

void Game::generateLevel() {
  Logging::debug("[Game::generateLevel] Generate level %d\n", currentLevel);
  delete_ptr(level);
  auto [timeBetweenObstacles, obstacleSpeed, numberOfLanes] = Constants::LEVELS_CONFIG[currentLevel - 1];
  if (difficulty == 2) {
    timeBetweenObstacles *= 0.8f;
    obstacleSpeed *= 1.2f;
  } else if (difficulty == 3) {
    timeBetweenObstacles *= 0.6f;
    obstacleSpeed *= 1.5f;
  }
  level = new Level(timeBetweenObstacles, obstacleSpeed, numberOfLanes, player, std::chrono::steady_clock::now().time_since_epoch().count());
  trafficLight->reset();
}
