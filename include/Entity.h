#pragma once
#include "Position.h"
#include "Utils.h"

class PlayerContext;

class Entity {
  /**
  * Entity: Represents a movable agent on the map
  */
protected:
  PlayerContext* pContext;

public:
  Entity(PlayerContext* pContext, Position p) : pContext(pContext), position(p) {}
  Position position;

  virtual void render() {};
};


class Troop : public Entity {
  /**
  * [Troop]: basic soldier
  */
public:
  Troop(PlayerContext* pContext, Position p) : Entity(pContext, p) {}
  void render() override;
};
