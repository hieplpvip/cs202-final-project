#ifndef GAME_H
#define GAME_H

// clang-format off
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
// clang-format on

#include "Level.h"
#include "Player.h"
#include "TrafficLight.h"

class Game : public olc::PixelGameEngine {
public:
  Game();

  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  bool OnUserDestroy() override;

  void newGame();
  void nextLevel();
  void generateLevel();
  void gotoxy(int x, int y);
  void load();
  void loading();

private:
  enum {
    GAME_STATE_TITLE,
    GAME_STATE_MENU,
    GAME_STATE_LOADING,
    GAME_STATE_PLAY,
    GAME_STATE_WIN,
    GAME_STATE_GAMEOVER,
    GAME_STATE_LOADGAME,
    GAME_STATE_SETTINGS,
    GAME_STATE_PAUSE,
    GAME_SAVE
  } gameState;

  int sound = 1;
  float timeAccumulator;
  int selectedMenuItem;
  int selectedSettingItem;
  int selectedPauseItem;
  int selectedLoadItem;
  int currentLevel;
  int coinEaten;

  Player* player;
  TrafficLight* trafficLight;
  Level* level;
};

#endif
