#ifndef GAME_H
#define GAME_H

// clang-format off
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
#include "olcPGEX_Font.h"
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
  void load();

private:
  enum {
    GAME_STATE_LOADING,
    GAME_STATE_TITLE,
    GAME_STATE_MENU,
    GAME_STATE_NEWGAME,
    GAME_STATE_NEXTLEVEL,
    GAME_STATE_PLAY,
    GAME_STATE_WIN,
    GAME_STATE_GAMEOVER,
    GAME_STATE_SAVEGAME,
    GAME_STATE_LOADGAME,
    GAME_STATE_SETTINGS,
    GAME_STATE_PAUSE,
  } gameState;

  float timeAccumulator;
  int selectedMenuItem;
  int selectedSettingItem;
  int selectedPauseItem;
  int selectedLoadItem;
  int difficulty;
  int soundEnabled;
  int currentSound;
  int currentLevel;
  int currentPoints;
  int coinEaten;

  Player* player;
  TrafficLight* trafficLight;
  Level* level;

  static int sndIntro;
  static int sndInGame;
  static olc::Font* erasFont;
};

#endif
