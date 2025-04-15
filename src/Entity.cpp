#include "Entity.h"
#include "Constants.h"
#include "GameContext.h"
#include "Utils.h"
#include <raylib.h>

Entity::Entity(PlayerContext *pContext, int entityId, Vector2 p)
    : pContext(pContext), entityId(std::to_string(pContext->playerId) + "_" +
                                   std::to_string(entityId)),
      position(p), targetPosition(p) {

  attackRange =  getRandDouble(TROOP_RENDER_RADIUS, 50);
}

void Entity::render() {
  // Render Health Bar and Box
  DrawRectangle(position.x - TROOP_RENDER_RADIUS,
                position.y - TROOP_RENDER_RADIUS - TROOP_HEALTH_PADDING,
                ((float)health / MAX_HEALTH) * TROOP_RENDER_RADIUS * 2,
                TROOP_RENDER_RADIUS / 2, RED);
  DrawRectangleLines(position.x - TROOP_RENDER_RADIUS,
                     position.y - TROOP_RENDER_RADIUS - TROOP_HEALTH_PADDING,
                     TROOP_RENDER_RADIUS * 2, TROOP_RENDER_RADIUS / 2, BLACK);
}

void Entity::setTargetPosition(Vector2 &newPos) { targetPosition = newPos; }

void Entity::onTickUpdate(float tsDelta) {
  // move towards target position
  if (Vector2Equals(position, targetPosition)) {
    return;
  }

  float totalDist = Vector2Distance(position, targetPosition);

  float moveDist = VELOCITY * tsDelta;
  float moveRatio = std::abs(moveDist / totalDist);

  if (moveRatio > 1) {
    return;
  }

  position.x += (targetPosition.x - position.x) * moveRatio;
  position.y += (targetPosition.y - position.y) * moveRatio;
}

Troop::Troop(PlayerContext *pContext, int entityId, Vector2 p)
    : Entity(pContext, entityId, p) {
  // Randomly initialize some attack values
  attackPower = GetRandomValue(1, 10);
  attackRate = GetRandomValue(1, 5);
}

void Troop::render() {
  Entity::render();
  // Draw Troop
  DrawCircle(position.x, position.y, TROOP_RENDER_RADIUS, pContext->color);

  // Draw Troop Attack Range
  DrawCircleLines(position.x, position.y, attackRange, pContext->color);

  // Draw line to target position
  DrawLine(position.x, position.y, targetPosition.x, targetPosition.y, pContext->color);
}

void Troop::onCollision(Entity &other, float ts) {
  if (other.pContext->playerId != pContext->playerId) {
    other.health =
        std::max(0, other.health -= calcDmg(ts)); // attack other player
  }
}

// TODO: simple way to reduce the number of attacks being done
float Troop::calcDmg(float ts) {
  if (ts - prevAttackTs < attackRate) {
    return 0;
  }
  prevAttackTs = ts;
  return attackPower;
}

// JSON
void to_json(nlohmann::json &j, const Vector2 &v) {
  j["x"] = v.x;
  j["y"] = v.y;
}

void from_json(const nlohmann::json &j, Vector2 &v) {
  v.x = j["x"];
  v.y = j["y"];
}

void to_json(nlohmann::json &j, const Entity &e) {
  j["entityId"] = e.entityId;
  j["health"] = e.health;
  j["attackPower"] = e.attackPower;
  j["attackRate"] = e.attackRate;
  j["position"] = e.position;
  j["isDead"] = e.isDead();
  j["attackRange"] = e.attackRange;
}

void from_json(const nlohmann::json &j, Entity &e) {
  j.at("entityId").get_to(e.entityId);
  j.at("position").get_to(e.position);
}
