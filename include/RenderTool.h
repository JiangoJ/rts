#pragma once

#include "GameContext.h"
#include "Utils.h"

struct ToolButton {
  std::string text{};
  Rectangle bounds;
  bool isPressed{};

  Color getColor() {
    return isPressed ? RED : WHITE;
  }
};

class RenderTool {

  /**
   * [RenderTool]: Helper GUI for placing individual elements on the game map
   * for testing Capabilities:
   * - Play, Pause
   * - Place Troops, Place Obstances, Place Resources
   */

private:
  static bool isLaunched;
  static GameContext* gContext;

  // Buttons
  static std::vector<ToolButton> buttons;
  static std::optional<int> selectedPContext;

public:
  static void setGameContext(GameContext* gContext);
  static void launchTool();
  static void closeTool();

  static void renderTool();
};
