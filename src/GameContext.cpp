#include "GameContext.h"
#include "Constants.h"
#include "Utils.h"
#include <raylib.h>

PlayerContext::PlayerContext() {
  // Randomly initialize a starting color
  color = Color{
    (unsigned char) GetRandomValue(0, 255), 
    (unsigned char) GetRandomValue(0, 255), 
    (unsigned char) GetRandomValue(0, 255), 255
  };
}

/**
 * [For Testing]: create a bunch of random entities
 */
void PlayerContext::randomInitialization(int entityCount) {

  assert(entities.empty());

  entities.reserve(entityCount);
  for (int i = 0; i < entityCount; i++) {
    auto randomX = getRandDouble(0, WIN_WIDTH);
    auto randomY = getRandDouble(0, WIN_HEIGHT);
    entities.emplace_back(new Troop(this, Position(randomX, randomY)));
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

// TODO: currently just randomly moves around, on average should stay in place
void GameContext::updateTick(float ts) {

  float tsDelta = ts - currTs;
  currTs = ts;

  // Temp: just to get some random movements
  for (auto &pContext : playerContexts) {
    for (auto &e : pContext.entities) {
      auto xDelta = getRandDouble(-50, 50) / 15;
      auto yDelta = getRandDouble(-50, 50) / 15;
      e->position.x += xDelta;
      e->position.y += yDelta;
    }
  }
}

void GameContext::initialize(int playerCount) {
  assert(playerContexts.empty());

  playerContexts.reserve(playerCount);
  for (int i = 0; i < playerCount; i++) {
    playerContexts.emplace_back();
  }
}
