#pragma once
#include "Position.h"
#include "Utils.h"

class Entity {
  /**
  * Entity: Represents a movable agent on the map
  */

public:
  Entity(Position p) : position(p) {}
  Position position;
};


class Troop : public Entity {
  /**
  * [Troop]: basic soldier
  */
public:
  Troop(Position p) : Entity(p) {}
};
