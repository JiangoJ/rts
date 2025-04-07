#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <math.h>
#include <random>
#include <optional>

#include "Constants.h"

#include <raylib.h>

#include <nlohmann/json.hpp>

// Rand
float getRandDouble(int min, int max);
Color getRandomColor();
