#pragma once
#include "Utils.h"
#include "Entity.h"

class PlayerContext {
public:
  PlayerContext() {};

  std::vector<Entity> entities;
  void randomInitialization(int entityCount);
};

class GameContext {
  /**
  * [Game Context]: The overarching state of the game tracking all necessary infomration to maintain game functionality.
  * It is a superset of all Knowledge Contexts (players) along with map based information
  */
private:
  float currTs{};


public:
  GameContext(float startTs) : currTs(startTs) {};

  std::vector<PlayerContext> playerContexts;
  void randomInitialization(int playerCount, int entityCount);
  void updateTick(float newTs); // TODO: should run the game loop
};
