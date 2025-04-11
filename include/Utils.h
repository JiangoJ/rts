#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <math.h>
#include <cmath>
#include <random>
#include <optional>

#include "Constants.h"

#include <raylib.h>
#include <raymath.h>

#include <llama-cpp.h>
#include <llama.h>

#include <nlohmann/json.hpp>

#include <fmt/core.h>

#include <curl/curl.h>
#include <curl/easy.h>

// Rand
float getRandDouble(int min, int max);
Color getRandomColor();
