#pragma once
#include "Bike.h"
#include "Obstacle.h"
#include <vector>

struct PowerUp {
  int lane;
  float posZ;
  int type;
  bool active = true;

  glm::vec3 getPosition(float laneWidth) const {
    return glm::vec3((lane - 1) * laneWidth, 1.0f, posZ);
  }
};

class Level {
public:
  float laneWidth = 3.0f;
  std::vector<Obstacle> obstacles;
  std::vector<PowerUp> powerUps;

  float nextObstacleZ = -20.0f;
  int obstacleDifficulty = 0;

  Level();
  void generate();
  bool checkCollisions(const Bike &bike) const;
  int checkPowerUp(const Bike &bike);
  void update(float bikeZ);
  void reset();

private:
  bool aabbOverlap(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB,
                   glm::vec3 maxB) const;
};
