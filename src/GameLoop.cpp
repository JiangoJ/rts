#include "GameLoop.h"
#include "Constants.h"
#include "GameContext.h"
#include "LLMClient.h"
#include "RenderTool.h"

std::unique_ptr<GameContext> GameLoop::currContext = nullptr;
bool GameLoop::initialized = false;

GameContext *getContext() { return GameLoop::currContext.get(); }

void GameLoop::initialize() {

  // singleton
  assert(!initialized);

  InitWindow(WIN_WIDTH, WIN_HEIGHT, "rts");
  SetTargetFPS(60);

  initialized = true;

  // TODO: this client to gamecontext circular dependency is unideal
  // Initialize GroqClient
  LLMClient groqClient;

  // Start Game
  currContext = std::make_unique<GameContext>(GetFrameTime(), &groqClient);
  currContext->randomInitialization(2, 5);

  groqClient.setGameContext(currContext.get());

  // Give game context to render tool
  RenderTool::setGameContext(currContext.get());
  RenderTool::launchTool();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Update GameContext
    currContext->updateTick(GetTime());
    // Update Frame
    renderFrame();
    EndDrawing();
  }

  CloseWindow();
}

/**
 * Render all of the visual elements
 */
void GameLoop::renderFrame() {

  // Load tool
  RenderTool::renderTool();

  // For now just the entities
  for (auto &pContext : currContext->playerContexts) {
    for (auto &e : pContext.entities) {
      e->render();
    }
  }
}
