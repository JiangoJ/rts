#include "Entity.h"
#include "GameContext.h"

void Troop::render() {
  DrawCircle(position.x, position.y, 10, pContext->color);
}
