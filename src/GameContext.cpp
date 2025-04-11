#include "GameContext.h"
#include "Constants.h"
#include "Utils.h"
#include <raylib.h>

PlayerContext::PlayerContext(int playerId) : playerId(playerId) {
  // Randomly initialize a starting color
  color = getRandomColor();
}

void PlayerContext::addTroop(const Vector2 &position) {
  auto size = entities.size();
  entities.emplace_back(new Troop(this, size, position));
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
    entities.emplace_back(new Troop(this, i, Vector2{randomX, randomY}));
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

// Move entities toward their desired location
void GameContext::updateTick(float ts) {

  float tsDelta = ts - currTs;
  currTs = ts;

  if (currTs - lastQueryTs >= QUERY_DELTA_S || lastQueryTs == 0) {
    auto gameStateJson = nlohmann::json(*this);
    for (auto& pContext : playerContexts) {
      gameStateJson["activePlayer"] = pContext.playerId;
      client->queryRTS(gameStateJson);
      auto resJson = client->getRTSJsonResponse();
      updateGameState(resJson);
    }
    lastQueryTs = currTs;
  }

  for (auto &pContext : playerContexts) {
    for (auto &e : pContext.entities) {
      if (e->isDead()) {
        continue;
      }
      e->onTickUpdate(tsDelta);
    }
  }

  collisionDetection();
}

void GameContext::initialize(int playerCount) {
  assert(playerContexts.empty());

  playerContexts.reserve(playerCount);
  for (int i = 0; i < playerCount; i++) {
    playerContexts.emplace_back(i);
  }
}

// TODO: This is very inefficient O(n^2), ideally some quadtree implementation
// would be better
void GameContext::collisionDetection() {
  for (auto &pContext : playerContexts) {
    for (auto *e : pContext.entities) {

      if (e->isDead()) {
        continue;
      }

      for (auto &pContextOther : playerContexts) {
        for (auto *eOther : pContextOther.entities) {
          if (CheckCollisionCircles(e->position, TROOP_RENDER_RADIUS,
                                    eOther->position, TROOP_RENDER_RADIUS)) {
            e->onCollision(*eOther, currTs);
          }
        }
      }
    }
  }
}

void GameContext::updateGameState(nlohmann::json& updatedState) {
  int activePlayer = updatedState["activePlayer"];
  auto entities = updatedState["players"][activePlayer]["entities"].template get<std::vector<Entity>>();
  auto& player = playerContexts[activePlayer];
  for (int i = 0; i < player.entities.size(); i++) {
    auto* e = player.entities[i];
    auto& newE = entities[i];
    e->position.x = newE.position.x;
    e->position.y = newE.position.y;
  }
}

// JSON
void to_json(nlohmann::json &json, const PlayerContext &pc) {
  json["playerId"] = pc.playerId;
  for (auto *e : pc.entities) {
    json["entities"].push_back(*e);
  }
}

void to_json(nlohmann::json &json, const GameContext &gc) {
  json["activePlayer"] = gc.activePlayer;
  json["current_timestamp"] = gc.currTs;
  json["players"] = gc.playerContexts;
}
