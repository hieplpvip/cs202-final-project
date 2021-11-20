#ifndef GAME_H
#define GAME_H

// clang-format off
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
// clang-format on

class Game : public olc::PixelGameEngine {
public:
  Game();

  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  bool OnUserDestroy() override;

  void newGame();

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
  } gameState;

  float timeAccumulator;
  int selectedMenuItem;
  int currentLevel;
  int coinEaten;
};

#endif
