#include "RenderTool.h"
#include "Constants.h"
#include "GameContext.h"

// Static Init
bool RenderTool::isLaunched = false;
GameContext *RenderTool::gContext = nullptr;
std::vector<ToolButton> RenderTool::buttons = std::vector<ToolButton>();
std::optional<int> RenderTool::selectedPContext = std::nullopt;

void RenderTool::launchTool() {
  assert(!isLaunched && gContext);
  isLaunched = true;

  auto numPlayers = gContext->playerContexts.size();
  buttons.reserve(numPlayers);

  for (int i = 0; i < numPlayers; i++) {
    auto& pContext = gContext->playerContexts[i];
    ToolButton tb = {"P" + std::to_string(pContext.playerId), Rectangle{(float)i * (RENDER_TOOLBUTTON_WIDTH + 5) + 5, 5, RENDER_TOOLBUTTON_WIDTH, RENDER_TOOLBUTTON_HEIGHT}, 0};
    buttons.push_back(tb);
  }
}

void RenderTool::renderTool() {

  bool isCollisionWithButton = false;

  auto mousePoint = GetMousePosition();
  // Draw buttons for choosing which player to add troops for
  for (int i = 0; i < buttons.size(); i++) {
    auto& bTool = buttons[i];
    // Render Button
    DrawRectangle(bTool.bounds.x, bTool.bounds.y, bTool.bounds.width,
                  bTool.bounds.height, bTool.getColor());
    DrawText(bTool.text.c_str(), (int) (bTool.bounds.x + (bTool.bounds.width) / 3), (int) (bTool.bounds.y + (bTool.bounds.height) / 3), 5, BLACK);

    if (CheckCollisionPointRec(mousePoint, bTool.bounds)) {
      isCollisionWithButton = true;
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!selectedPContext) {
          bTool.isPressed = !bTool.isPressed;
          selectedPContext = i;
        }
        else if (selectedPContext == i){
          bTool.isPressed = !bTool.isPressed;
          if (!bTool.isPressed) {
            selectedPContext.reset();
          }
        }
      }
    }
  }

  // place troops, some logic to ensure we dont place extra troop when we click button
  if (selectedPContext && !isCollisionWithButton) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      gContext->playerContexts[selectedPContext.value()].addTroop(mousePoint);
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      gContext->playerContexts[selectedPContext.value()].removeLastTroop();
    }
  }
}

void RenderTool::closeTool() { isLaunched = false; }

void RenderTool::setGameContext(GameContext *gCxt) { gContext = gCxt; }
