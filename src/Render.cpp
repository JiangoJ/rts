#include "Render.h"

void renderEntity(Entity& e) {
  DrawCircle(e.position.x, e.position.y, 10, BLACK);
}
