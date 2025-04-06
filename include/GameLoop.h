#pragma once

#include "Utils.h"
#include "GameContext.h"
#include "Render.h"

class GameLoop {
  /**
  * [GameLoop]: GUI rendering, maintains context for running the game that does not involve any actual game logic
  * On each tick, GameLoop will query the GameContext to determine what it needs to draw
  */

private:
  static bool initialized;

public:
  static std::unique_ptr<GameContext> currContext;

  static void initialize();
  static GameContext* getContext();

  static void renderFrame();
};
