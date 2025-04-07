#include "Entity.h"
#include "GameContext.h"
#include "Utils.h"
#include <raylib.h>

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


Troop::Troop(PlayerContext* pContext, Vector2 p) : Entity(pContext, p) {
  attackPower = GetRandomValue(1, 10);
  attackRate = GetRandomValue(1, 5);
}

void Troop::render() {
  Entity::render();
  DrawCircle(position.x, position.y, TROOP_RENDER_RADIUS, pContext->color);
}

void Troop::onCollision(Entity& other, float ts) {
  // TODO: reduce the number of times attacks happen
  if (other.pContext->playerId != pContext->playerId) {
    other.health = std::max(0, other.health -= calcDmg(ts)); // attack other player
  }
}

float Troop::calcDmg(float ts) {
  if (ts - prevAttackTs < attackRate) {
    return 0;
  }
  prevAttackTs = ts;
  return attackPower;
}
