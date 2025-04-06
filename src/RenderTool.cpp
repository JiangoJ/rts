#include "RenderTool.h"
#include "Constants.h"
#include "GameContext.h"
#include <optional>

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
    ToolButton tb = {"P" + std::to_string(i), Rectangle{(float)i * (RENDER_TOOLBUTTON_WIDTH + 5) + 5, 5, RENDER_TOOLBUTTON_WIDTH, RENDER_TOOLBUTTON_HEIGHT}, 0};
    buttons.push_back(tb);
  }
}

void RenderTool::renderTool() {
  auto mousePoint = GetMousePosition();
  // Draw buttons for choosing which player to add troops for
  for (int i = 0; i < buttons.size(); i++) {
    auto& bTool = buttons[i];
    // Render Button
    DrawRectangle(bTool.bounds.x, bTool.bounds.y, bTool.bounds.width,
                  bTool.bounds.height, bTool.getColor());
    DrawText(bTool.text.c_str(), (int) (bTool.bounds.x + (bTool.bounds.width) / 3), (int) (bTool.bounds.y + (bTool.bounds.height) / 3), 5, BLACK);

    if (CheckCollisionPointRec(mousePoint, bTool.bounds)) {
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

  // May need to place troops
  if (selectedPContext) {

  }
}

void RenderTool::closeTool() { isLaunched = false; }

void RenderTool::setGameContext(GameContext *gCxt) { gContext = gCxt; }
