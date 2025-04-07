#pragma once
#include "Utils.h"
#include "Entity.h"


class PlayerContext {
  /**
   * [PlayerContext]: Subset of GameContext that represents knowledge of the player
   */
public:
  int playerId;
  Color color;
  std::vector<Entity*> entities;

  PlayerContext(int playerId);
  void randomInitialization(int entityCount);

  void addTroop(const Vector2& position);
  void removeLastTroop();
};

class GameContext {
  /**
  * [Game Context]: The overarching state of the game tracking all necessary infomration to maintain game functionality.
  * It is a superset of all Knowledge Contexts (players) along with map based information
  */
public:
  float currTs{};

  GameContext(float startTs) : currTs(startTs) {};
  std::vector<PlayerContext> playerContexts;

  void updateTick(float newTs); 
  void collisionDetection();


  void randomInitialization(int playerCount, int entityCount);
  void initialize(int playerCount);
};
