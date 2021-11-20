#include "Game.h"
#include "Constants.h"

#define delete_ptr(ptr) \
  if (ptr != nullptr) { \
    delete ptr;         \
    ptr = nullptr;      \
  }

Game::Game() {
  sAppName = "Crossing Road";
}

bool Game::OnUserCreate() {
  olc::SOUND::InitialiseAudio();

  gameState = GAME_STATE_TITLE;
  timeAccumulator = 0;
  selectedMenuItem = 0;
  currentLevel = 1;
  coinEaten = 0;

  player = new Player();

  return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
  Clear(olc::BLANK);
  timeAccumulator += fElapsedTime;

  switch (gameState) {
    case GAME_STATE_TITLE: {
      int titleScale = 2;
      olc::vi2d titleSize = GetTextSize(Constants::TITLE) * titleScale;
      olc::vi2d titlePos = ScreenSize() / 2 - titleSize / 2;
      DrawString(titlePos, Constants::TITLE, olc::RED, titleScale);

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
          newGame();
          return true;
        } else if (selectedMenuItem == 1) {
          // Load Game
          gameState = GAME_STATE_LOADGAME;
          timeAccumulator = 0;
        } else if (selectedMenuItem == 2) {
          // Settings
          gameState = GAME_STATE_SETTINGS;
          timeAccumulator = 0;
        } else {
          // Exit
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
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
        return true;
      }

      DrawString(0, 0, "Level: " + std::to_string(currentLevel), olc::RED);

      if (GetKey(olc::UP).bHeld) {
        player->moveUp(fElapsedTime);
      }
      if (GetKey(olc::DOWN).bHeld) {
        player->moveDown(fElapsedTime);
      }
      if (GetKey(olc::LEFT).bHeld) {
        player->moveLeft(fElapsedTime);
      }
      if (GetKey(olc::RIGHT).bHeld) {
        player->moveRight(fElapsedTime);
      }

      player->draw(this);

      break;
    }
    case GAME_STATE_WIN: {
      // TODO
      break;
    }
    case GAME_STATE_GAMEOVER: {
      // TODO
      break;
    }
    case GAME_STATE_LOADGAME: {
      DrawString(0, 0, "Work in progress", olc::RED);

      if (timeAccumulator > 2) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_SETTINGS: {
      DrawString(0, 0, "Work in progress", olc::RED);

      if (timeAccumulator > 2) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
  };

  return true;
}

bool Game::OnUserDestroy() {
  olc::SOUND::DestroyAudio();

  delete_ptr(player);

  return true;
}

void Game::newGame() {
  gameState = GAME_STATE_PLAY;
  timeAccumulator = 0;
  currentLevel = 1;
  coinEaten = 0;
}
