#include "GameContext.h"
#include "Constants.h"
#include "Utils.h"
#include <raylib.h>

PlayerContext::PlayerContext(int playerId) : playerId(playerId) {
  // Randomly initialize a starting color
  color = getRandomColor();
}

void PlayerContext::addTroop(const Vector2& position) {
  entities.emplace_back(new Troop(this, position));
}

void PlayerContext::removeLastTroop() {
  if (entities.empty()) {
    return;
  }
  entities.pop_back();
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
    entities.emplace_back(new Troop(this, Vector2{randomX, randomY}));
  }
}

/**
 * [For Testing]: create a bunch of random players and entities
 */
void GameContext::randomInitialization(int playerCount, int entityCount) {

  assert(playerContexts.empty());

  playerContexts.reserve(playerCount);
  for (int i = 0; i < playerCount; i++) {
    playerContexts.emplace_back(i);
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

      if (e->isDead()) {
        continue;
      }

      auto xDelta = getRandDouble(-50, 50) / 15;
      auto yDelta = getRandDouble(-50, 50) / 15;
      e->position.x += xDelta;
      e->position.y += yDelta;
    }
  }

  // TODO: collision detection
  collisionDetection();
}

void GameContext::initialize(int playerCount) {
  assert(playerContexts.empty());

  playerContexts.reserve(playerCount);
  for (int i = 0; i < playerCount; i++) {
    playerContexts.emplace_back(i);
  }
}

// TODO: This is very inefficient O(n^2), ideally some quadtree implementation would be better
void GameContext::collisionDetection() {
  for (auto& pContext : playerContexts) {
    for (auto* e : pContext.entities) {

      if (e->isDead()) {
        continue;
      }

      for (auto& pContextOther : playerContexts) {
        for (auto* eOther : pContextOther.entities) {
          if (CheckCollisionCircles(e->position, TROOP_RENDER_RADIUS, eOther->position, TROOP_RENDER_RADIUS)) {
            e->onCollision(*eOther, currTs);
          }
        }
      }
    }
  }
}
