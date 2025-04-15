#include "GameContext.h"
#include "Constants.h"
#include "Utils.h"

PlayerContext::PlayerContext(int playerId) : playerId(playerId) {
  // Randomly initialize a starting color
  color = getRandomColor();
}

void PlayerContext::addTroop(const Vector2 &position) {
  Troop* newTroop = new Troop(this, entityMap.size(), position);
  entityMap.emplace(newTroop->entityId, newTroop);
}

void PlayerContext::removeLastTroop() {
  if (entityMap.empty()) {
    return;
  }

  entityMap.erase(--entityMap.end());
}

/**
 * [For Testing]: create a bunch of random entities
 */
void PlayerContext::randomInitialization(int entityCount) {

  assert(entityMap.empty());

  for (int i = 0; i < entityCount; i++) {
    auto randomX = getRandDouble(0, WIN_WIDTH);
    auto randomY = getRandDouble(0, WIN_HEIGHT);
    addTroop(Vector2{randomX, randomY});
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
      if (pContext.entityMap.empty()) {
        continue;
      }
      gameStateJson["activePlayer"] = pContext.playerId;
      client->queryRTS(gameStateJson);
    }
    lastQueryTs = currTs;
  }

  for (auto &pContext : playerContexts) {
    for (auto &[eId, e] : pContext.entityMap) {
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
    for (auto& [eId, e] : pContext.entityMap) {

      if (e->isDead()) {
        continue;
      }

      for (auto &pContextOther : playerContexts) {
        for (auto& [eOtherId, eOther] : pContextOther.entityMap) {
          if (CheckCollisionCircles(e->position, e->attackRange,
                                    eOther->position, TROOP_RENDER_RADIUS)) {
            e->onCollision(*eOther, currTs);
          }
        }
      }
    }
  }
}

void GameContext::updateGameState(const nlohmann::json& updatedState) {
  int activePlayer = updatedState["activePlayer"];
  auto entities = updatedState["entities"].template get<std::vector<Entity>>();
  auto& player = playerContexts[activePlayer];

  for (auto& updatedEntity : entities) {

    auto eIt = player.entityMap.find(updatedEntity.entityId);
    if (eIt == player.entityMap.end()) {
      continue;
    }
    eIt->second->setTargetPosition(updatedEntity.position);
  }
}

// JSON
void to_json(nlohmann::json &json, const PlayerContext &pc) {
  json["playerId"] = pc.playerId;
  for (auto& [eId, e] : pc.entityMap) {
    json["entities"].push_back(*e);
  }
}

void to_json(nlohmann::json &json, const GameContext &gc) {
  json["activePlayer"] = gc.activePlayer;
  json["players"] = gc.playerContexts;
}
