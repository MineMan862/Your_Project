#include "Obstacle.h"
#include <cstdlib>

Obstacle::Obstacle()
    : lane(1), posZ(0.0f), type(OBSTACLE_TALL), height(2.5f),
      color(1.0f, 0.0f, 0.0f), rotationY(90.0f) {}

Obstacle::Obstacle(int lane, float posZ, ObstacleType type, float laneWidth)
    : lane(lane), posZ(posZ), type(type) {
  if (type == OBSTACLE_TALL) {
    height = 2.5f;
    color = glm::vec3(0.85f, 0.15f, 0.15f); // Red
    rotationY = static_cast<float>(rand() % 360);
  } else {
    height = 0.6f;
    color = glm::vec3(0.9f, 0.6f, 0.1f); // Orange
    rotationY = 0.0f;
  }
}

glm::vec3 Obstacle::getPosition(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  return glm::vec3(x, height / 2.0f, posZ);
}

glm::vec3 Obstacle::getScale() const { return glm::vec3(width, height, depth); }

glm::vec3 Obstacle::getAABBMin(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  return glm::vec3(x - width / 2.0f, 0.0f, posZ - depth / 2.0f);
}

glm::vec3 Obstacle::getAABBMax(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  return glm::vec3(x + width / 2.0f, height, posZ + depth / 2.0f);
}
