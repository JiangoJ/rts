#include "Utils.h"
#include <random>

double getRandDouble(int min, int max) {

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(min, max);

  return distrib(gen);
}
