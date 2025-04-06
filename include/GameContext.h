#pragma once
#include "Utils.h"
#include "Entity.h"

class PlayerContext {
  /**
   * [PlayerContext]: Subset of GameContext that represents knowledge of the player
   */
public:
  Color color;

  PlayerContext();
  std::vector<Entity*> entities;
  void randomInitialization(int entityCount);

  void addTroop(const Vector2& position);
  void removeLastTroop();
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

  void initialize(int playerCount);
};
