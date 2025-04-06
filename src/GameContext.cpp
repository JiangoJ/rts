#include "GameContext.h"
#include "Constants.h"
#include "Utils.h"

/**
* [For Testing]: create a bunch of random entities
*/
void PlayerContext::randomInitialization(int entityCount) {

  assert(entities.empty());

  entities.reserve(entityCount);
  for (int i = 0; i < entityCount; i++) {
    auto randomX = getRandDouble(0, WIN_WIDTH);
    auto randomY = getRandDouble(0, WIN_HEIGHT);
    entities.emplace_back(Position(randomX, randomY));
  }
}

/**
* [For Testing]: create a bunch of random players and entities
*/
void GameContext::randomInitialization(int playerCount, int entityCount) {
  
  assert(playerContexts.empty());

  playerContexts.reserve(playerCount);
  for (int i = 0; i < playerCount; i++) {
    playerContexts.emplace_back();
    playerContexts.back().randomInitialization(entityCount);
  }
}

void GameContext::updateTick(float ts) {

  float tsDelta = ts - currTs;
  currTs = ts;

  // Some randomization for movement
  for (auto& pContext : playerContexts) {
    for (auto& e : pContext.entities) {
      auto xDelta = getRandDouble(-40, 50) / 10;
      auto yDelta = getRandDouble(-50, 40) / 10;
      e.position.x += xDelta;
      e.position.y += yDelta;
    }
  }
}
