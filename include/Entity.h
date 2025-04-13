#pragma once
#include "Utils.h"

class PlayerContext;

constexpr int MAX_HEALTH = 100;
constexpr float VELOCITY = 10; // pixels per tick
constexpr float MIN_MOVE_DIST = 1; // pixels per tick

class Entity {
  /**
   * Entity: Represents a movable agent on the map
   */
public:
  PlayerContext *pContext;
  std::string entityId;

  int health{MAX_HEALTH};
  int attackPower;  // dmg per attack
  float attackRate; // s between attacks
  float prevAttackTs{};

  Entity() {}
  Entity(PlayerContext *pContext, int entityId, Vector2 p);
  Vector2 position;
  Vector2 targetPosition;

  // Some common rendering for all entities
  virtual void render();
  virtual void onCollision(Entity &other, float ts) {}
  virtual float calcDmg(float ts) { return 0; }

  bool isDead() const { return health <= 0; }
  void setTargetPosition(Vector2& newPos);
  virtual void onTickUpdate(float tsDelta);
};

class Troop : public Entity {
  /**
   * [Troop]: basic soldier
   */
public:
  Troop(PlayerContext *pContext, int entityId, Vector2 p);

  void render() override;
  void onCollision(Entity &other, float ts) override;
  float calcDmg(float ts) override;
};

// JSON
void to_json(nlohmann::json& json, const Vector2& v);
void to_json(nlohmann::json& json, const Entity& e);
void from_json(const nlohmann::json& json, Vector2& e);
void from_json(const nlohmann::json& json, Entity& e);
