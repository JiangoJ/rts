#pragma once
#include "Utils.h"

class PlayerContext;

constexpr int MAX_HEALTH = 100;

class Entity {
  /**
   * Entity: Represents a movable agent on the map
   */
public:
  PlayerContext *pContext;
  int health{MAX_HEALTH};
  int attackPower;  // dmg per attack
  float attackRate; // s between attacks
  float prevAttackTs{};

  Entity(PlayerContext *pContext, Vector2 p)
      : pContext(pContext), position(p) {}
  Vector2 position;

  // Some common rendering for all entities
  virtual void render();
  virtual void onCollision(Entity &other, float ts) {}
  virtual float calcDmg(float ts) { return 0; }

  bool isDead() { return health <= 0; }
};

class Troop : public Entity {
  /**
   * [Troop]: basic soldier
   */
public:
  Troop(PlayerContext *pContext, Vector2 p);

  void render() override;
  void onCollision(Entity &other, float ts) override;
  float calcDmg(float ts) override;
};
