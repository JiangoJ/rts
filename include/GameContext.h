#pragma once
#include "LLMClient.h"
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

  PlayerContext() {}
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
  float lastQueryTs{};
  int activePlayer{};
  LLMClient* client{};

  GameContext(float startTs, LLMClient* client) : currTs(startTs), client(client) {};
  std::vector<PlayerContext> playerContexts;

  void updateTick(float newTs); 
  void collisionDetection();
  void updateGameState(nlohmann::json& updatedState);

  void randomInitialization(int playerCount, int entityCount);
  void initialize(int playerCount);
};

// void from_json(const nlohmann::json& json, PlayerContext& pc);
void to_json(nlohmann::json& json, const PlayerContext& pc);
void to_json(nlohmann::json& json, const GameContext& gc);
