#include "Game.h"
#include <cassert>
#include "Constants.h"
#include "Logging.h"
#include <iostream>
#include <future>
#include <thread>
#include <Windows.h>
#include <windows.h>
#include <mmsystem.h>
using namespace std;

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

void Game::gotoxy(int x, int y)
{
    COORD d;
    d.X = x;
    d.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), d);
}

void Game::load()
{
    float progress = 0.0;

    while (progress < 1.0) {

        int barWidth = 70;
        int pos = barWidth * progress;
        gotoxy(25, 16);

        Sleep(50);

        cout << "[";
        for (int i = 0; i < barWidth; i++) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "]" << int(progress * 100.0) << " %\r";
        cout.flush();

        progress += 0.01;
    }
    cout << endl;
}

void Game::loading()
{
    PlaySound(TEXT("MenuSound.wav"), NULL, SND_LOOP | SND_ASYNC);
    gotoxy(55, 14);
    cout << "Loading..." << endl;
    //gotoxy(10, 16);
    load();
}

bool Game::OnUserCreate() {
  Logging::debug("[Game::OnUserCreate] Initializing game engine\n");

  olc::SOUND::InitialiseAudio();

  gameState = GAME_STATE_TITLE;
  timeAccumulator = 0;
  selectedMenuItem = 0;
  selectedPauseItem = 0;
  selectedSettingItem = 0;
  currentLevel = 1;
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
      int titleScale = 2;
      olc::vi2d titleSize = GetTextSize(Constants::TITLE) * titleScale;
      olc::vi2d titlePos = ScreenSize() / 2 - titleSize / 2;
      DrawString(titlePos, Constants::TITLE, olc::RED, titleScale);

      if (timeAccumulator > Constants::TITLE_DURATION || GetMouse(0).bPressed || GetKey(olc::SPACE).bPressed) {
        gameState = GAME_STATE_MENU;
        //timeAccumulator = 0;
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

      DrawString(10, 10, "Level: " + std::to_string(currentLevel), olc::RED);
      DrawString(10, 20, "Score: " + std::to_string(coinEaten * 10), olc::RED);

      int dx = 0, dy = 0;
      if (GetKey(olc::LEFT).bHeld) --dx;
      if (GetKey(olc::RIGHT).bHeld) ++dx;
      if (GetKey(olc::UP).bHeld) --dy;
      if (GetKey(olc::DOWN).bHeld) ++dy;
      if (dy==-1) {
        player->moveUp(fElapsedTime);
      }
      if (dy==1) {
        player->moveDown(fElapsedTime);
      }
      if (dx==-1) {
        player->moveLeft(fElapsedTime);
      }
      if (dx==1) {
        player->moveRight(fElapsedTime);
      }

      trafficLight->update(fElapsedTime);
      if (trafficLight->getColor() != RED) {
          level->update(fElapsedTime);
      }

      level->draw(pge);
      trafficLight->draw(pge);
      player->draw(this);

      break;
    }
    case GAME_STATE_PAUSE: {
        if (GetKey(olc::ENTER).bPressed) {
            if (selectedPauseItem == 0) {
                // Continue
                gameState = GAME_STATE_PLAY;
            }
            else if(selectedPauseItem == 1) {
                // Back to menu
                selectedLoadItem = 0;
                gameState = GAME_SAVE;
            }
            else
            {
                gameState = GAME_STATE_MENU;
            }
        }
        else if (GetKey(olc::DOWN).bPressed) {
            ++selectedPauseItem;
            if (selectedPauseItem >= Constants::PAUSE_ITEMS.size()) {
                selectedPauseItem = 0;
            }
        }
        else if (GetKey(olc::UP).bPressed) {
            --selectedPauseItem;
            if (selectedPauseItem < 0) {
                selectedPauseItem = (int)Constants::PAUSE_ITEMS.size() - 1;
            }
        }
        //cout << selectedSettingItem << endl;
        int height = GetTextSize(Constants::PAUSE_ITEMS[0]).y * 2;
        int offset = (ScreenHeight() - height * (int)Constants::PAUSE_ITEMS.size()) / 2;
        for (int i = 0; i < (int)Constants::PAUSE_ITEMS.size(); ++i) {
            float textScale = (i == selectedPauseItem) ? 1.5f : 1.2f;
            olc::Pixel textColor = (i == selectedPauseItem) ? olc::YELLOW : olc::WHITE;

            olc::vi2d center = { ScreenWidth() / 2, offset + height * (2 * i + 1) / 2 };
            olc::vi2d textSize = (olc::vf2d)(GetTextSize(Constants::PAUSE_ITEMS[i])) * textScale;
            olc::vi2d textPos = center - textSize / 2;
            DrawStringDecal(textPos, Constants::PAUSE_ITEMS[i], textColor, { textScale, textScale });
        }
    }
    case GAME_STATE_WIN: {
      // TODO
      break;
    }
    case GAME_STATE_GAMEOVER: {
      // TODO
      break;
    }
    case GAME_SAVE: {
        ifstream f("src/SaveGame/checkpoint.txt");
        std::vector<std::string> LOAD_ITEMS;
        string input;
        while (getline(f , input))
        {
            LOAD_ITEMS.push_back(input);
        }
        LOAD_ITEMS.push_back("Back To Pause");
        f.close();
          if (GetKey(olc::ENTER).bPressed) {
              if (selectedLoadItem == 0) {
                  // Save 1
                  LOAD_ITEMS[0] = "Save_1";
              }
              else if (selectedLoadItem == 1) {
                  // Save 2
                  LOAD_ITEMS[1] = "Save_2";
              }
              else if (selectedLoadItem == 2) {
                  // Save 3
                  LOAD_ITEMS[2] = "Save_3";
              }
              if (selectedLoadItem == 3)
              {
                  gameState = GAME_STATE_PAUSE;
              }
              else
              {
                  ofstream fo("src/SaveGame/checkpoint.txt");
                  for (int i = 0; i < 3; i++)
                      fo << LOAD_ITEMS[i] << endl;
                  fo.close();
                  ofstream fout("src/SaveGame/" + LOAD_ITEMS[selectedLoadItem] + ".dat", ios::binary);
                  fout.write((char*)&currentLevel, sizeof(currentLevel));
                  int score = coinEaten * 10;
                  fout.write((char*)&score, sizeof(score));
                  int X = player->getX();
                  fout.write((char*)&X, sizeof(X));
                  int Y = player->getY();
                  fout.write((char*)&Y, sizeof(Y));
                  fout.close();
              }
          }
          else if (GetKey(olc::DOWN).bPressed) {
              ++selectedLoadItem;
              if (selectedLoadItem >= LOAD_ITEMS.size()) {
                  selectedLoadItem = 0;
              }
          }
          else if (GetKey(olc::UP).bPressed) {
              --selectedLoadItem;
              if (selectedLoadItem < 0) {
                  selectedLoadItem = (int)LOAD_ITEMS.size() - 1;
              }
          }
      //cout << selectedSettingItem << endl;
      int height = GetTextSize(LOAD_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)LOAD_ITEMS.size()) / 2;
      for (int i = 0; i < (int)LOAD_ITEMS.size(); ++i) {
          float textScale = (i == selectedLoadItem) ? 1.5f : 1.2f;
          olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

          olc::vi2d center = { ScreenWidth() / 2, offset + height * (2 * i + 1) / 2 };
          olc::vi2d textSize = (olc::vf2d)(GetTextSize(LOAD_ITEMS[i])) * textScale;
          olc::vi2d textPos = center - textSize / 2;
          DrawStringDecal(textPos, LOAD_ITEMS[i], textColor, { textScale, textScale });
      }

      if (timeAccumulator > 101) {
        gameState = GAME_STATE_MENU;
        timeAccumulator = 0;
      }

      break;
    }
    case GAME_STATE_LOADGAME: {
        ifstream f("src/SaveGame/checkpoint.txt");
        std::vector<std::string> LOAD_ITEMS;
        string input;
        while (getline(f, input))
        {
            //cout << input << endl;
            LOAD_ITEMS.push_back(input);
        }
        LOAD_ITEMS.push_back("Back To Menu");
      if (GetKey(olc::ENTER).bPressed) {
          if (selectedLoadItem == 3)
          {
              gameState = GAME_STATE_MENU;
          }
          else
          {
              ifstream fin("src/SaveGame/" + LOAD_ITEMS[selectedLoadItem] + ".dat", ios::out | ios::binary);
              fin.read((char*)&currentLevel, sizeof(currentLevel));
              fin.read((char*)&coinEaten, sizeof(coinEaten));
              int k = player->getX();
              fin.read((char*)&k, sizeof(k));
              player->setX(k);
              k = player->getY();
              fin.read((char*)&k, sizeof(k));
              player->setY(k);
              gameState = GAME_STATE_PLAY;
          }
      }
      else if (GetKey(olc::DOWN).bPressed) {
          ++selectedLoadItem;
          if (selectedLoadItem >= LOAD_ITEMS.size()) {
              selectedLoadItem = 0;
          }
      }
      else if (GetKey(olc::UP).bPressed) {
          --selectedLoadItem;
          if (selectedLoadItem < 0) {
              selectedLoadItem = (int)LOAD_ITEMS.size() - 1;
          }
      }
      //cout << selectedSettingItem << endl;
      int height = GetTextSize(LOAD_ITEMS[0]).y * 2;
      int offset = (ScreenHeight() - height * (int)LOAD_ITEMS.size()) / 2;
      for (int i = 0; i < (int)LOAD_ITEMS.size(); ++i) {
          float textScale = (i == selectedLoadItem) ? 1.5f : 1.2f;
          olc::Pixel textColor = (i == selectedLoadItem) ? olc::YELLOW : olc::WHITE;

          olc::vi2d center = { ScreenWidth() / 2, offset + height * (2 * i + 1) / 2 };
          olc::vi2d textSize = (olc::vf2d)(GetTextSize(LOAD_ITEMS[i])) * textScale;
          olc::vi2d textPos = center - textSize / 2;
          DrawStringDecal(textPos, LOAD_ITEMS[i], textColor, { textScale, textScale });
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
                }
                else if (selectedSettingItem == 1) {
                    // Change Sound
                    if (sound == 0)
                        sound = 1;
                    else
                        sound = 0;
                    if (sound == 0)
                        PlaySound(NULL , NULL, NULL);
                    else
                        PlaySound(TEXT("MenuSound.wav"), NULL, SND_LOOP | SND_ASYNC);
              
                }
                else
                    timeAccumulator = 101;

                
            }
            else if (GetKey(olc::DOWN).bPressed) {
                ++selectedSettingItem;
                if (selectedSettingItem >= Constants::SETTING_ITEMS.size()) {
                    selectedSettingItem = 0;
                }
            }
            else if (GetKey(olc::UP).bPressed) {
                --selectedSettingItem;
                if (selectedSettingItem < 0) {
                    selectedSettingItem = (int)Constants::SETTING_ITEMS.size() - 1;
                }
            }
            //cout << selectedSettingItem << endl;
            int height = GetTextSize(Constants::SETTING_ITEMS[0]).y * 2;
            int offset = (ScreenHeight() - height * (int)Constants::SETTING_ITEMS.size()) / 2;
            for (int i = 0; i < (int)Constants::SETTING_ITEMS.size(); ++i) {
                float textScale = (i == selectedSettingItem) ? 1.5f : 1.2f;
                olc::Pixel textColor = (i == selectedSettingItem) ? olc::YELLOW : olc::WHITE;

                olc::vi2d center = { ScreenWidth() / 2, offset + height * (2 * i + 1) / 2 };
                olc::vi2d textSize = (olc::vf2d)(GetTextSize(Constants::SETTING_ITEMS[i])) * textScale;
                olc::vi2d textPos = center - textSize / 2;
                if (i == 1)
                {
                    if (sound == 0)
                        DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + "OFF", textColor, { textScale, textScale });
                    else
                        DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + "ON", textColor, { textScale, textScale });
                }
                else if (i == 0)
                {
                    DrawStringDecal(textPos, Constants::SETTING_ITEMS[i] + to_string(currentLevel), textColor, { textScale, textScale });
                }
                DrawStringDecal(textPos, Constants::SETTING_ITEMS[i], textColor, { textScale, textScale });
            }
      //DrawString(0, 0, "Work in progress", olc::RED);

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

  olc::SOUND::DestroyAudio();

  delete_ptr(player);

  return true;
}

void Game::newGame() {
  gameState = GAME_STATE_PLAY;
  timeAccumulator = 0;
  player -> init();
  //trafficLight = new TrafficLight();
  coinEaten = 0;
}

void Game::generateLevel()
{
    Logging::debug("[game::generateLevel] Generate level %d\n", currentLevel);
    delete_ptr(level);
    auto [timeBetweenObstacles, obstacleSpeed, numberOfLanes] = Constants::LEVELS_CONFIG[currentLevel - 1];
    level = new Level(timeBetweenObstacles, obstacleSpeed, numberOfLanes, player, currentLevel);
    trafficLight->reset();
}