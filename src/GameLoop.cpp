#include "GameLoop.h"
#include "GameContext.h"
#include "Render.h"
#include <raylib.h>

std::unique_ptr<GameContext> GameLoop::currContext = nullptr;
bool GameLoop::initialized = false;

GameContext *getContext() { return GameLoop::currContext.get(); }

void GameLoop::initialize() {

  // singleton
  assert(!initialized);

  InitWindow(WIN_WIDTH, WIN_HEIGHT, "rts");
  SetTargetFPS(60);

  initialized = true;

  // Start Game
  currContext = std::make_unique<GameContext>(GetFrameTime());
  currContext->randomInitialization(2, 5);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // TODO: draw
    // Update GameContext
    // Update Frame

    currContext->updateTick(GetFrameTime());
    renderFrame();
    EndDrawing();
  }

  CloseWindow();
}

/**
 * Render all of the visual elements
 */
void GameLoop::renderFrame() {
  // For now just the entities
  for (auto &pContext : currContext->playerContexts) {
    for (auto &e : pContext.entities) {
      renderEntity(e);
    }
  }
}
