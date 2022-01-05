#ifndef GAME_H
#define GAME_H

// clang-format off
#include "olcPixelGameEngine.h"
#include "olcPGEX_Font.h"
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

private:
  enum {
    GAME_STATE_LOADING,
    GAME_STATE_TITLE,
    GAME_STATE_MENU,
    GAME_STATE_NEWGAME,
    GAME_STATE_NEXTLEVEL,
    GAME_STATE_PLAY,
    GAME_STATE_WIN,
    GAME_STATE_HIT,
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
  int currentLives;
  int currentLevel;
  int currentPoints;
  int coinEaten;
  int highScore;

  Player* player;
  TrafficLight* trafficLight;
  Level* level;

  static int sndIntro;
  static int sndInGame;
  static olc::Font* erasFont;

  void newGame();
  void nextLevel();
  void generateLevel();
  void playSound(int snd);
  void readHighScore();
  void writeHighScore();
};

#endif
