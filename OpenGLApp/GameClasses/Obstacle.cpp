#include "Obstacle.h"
#include <cstdlib>
#include <cmath>

Obstacle::Obstacle()
    : lane(1), posZ(0.0f), type(OBSTACLE_TALL), height(2.5f),
      color(1.0f, 0.0f, 0.0f), rotationY(90.0f) {}

Obstacle::Obstacle(int lane, float posZ, ObstacleType type, float laneWidth)
    : lane(lane), posZ(posZ), type(type) {
  if (type == OBSTACLE_TALL) {
    height = 2.5f;
    color = glm::vec3(0.85f, 0.15f, 0.15f);
    // Permetti ai veicoli di spawnare in rotazioni casuali (ostacoli storti)
    rotationY = static_cast<float>(rand() % 360);
  } else {
    height = 0.6f;
    color = glm::vec3(0.9f, 0.6f, 0.1f);
    // Le macerie possono essere ruotate a caso
    rotationY = static_cast<float>(rand() % 360);
  }
}

glm::vec3 Obstacle::getPosition(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  return glm::vec3(x, height / 2.0f, posZ);
}

glm::vec3 Obstacle::getScale() const { return glm::vec3(width, height, depth); }

glm::vec3 Obstacle::getAABBMin(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  float rad = glm::radians(rotationY);
  float half_w = std::abs(std::cos(rad) * (width / 2.0f)) + std::abs(std::sin(rad) * (depth / 2.0f));
  float half_d = std::abs(std::sin(rad) * (width / 2.0f)) + std::abs(std::cos(rad) * (depth / 2.0f));
  return glm::vec3(x - half_w, 0.0f, posZ - half_d);
}

glm::vec3 Obstacle::getAABBMax(float laneWidth) const {
  float x = (lane - 1) * laneWidth;
  float rad = glm::radians(rotationY);
  float half_w = std::abs(std::cos(rad) * (width / 2.0f)) + std::abs(std::sin(rad) * (depth / 2.0f));
  float half_d = std::abs(std::sin(rad) * (width / 2.0f)) + std::abs(std::cos(rad) * (depth / 2.0f));
  return glm::vec3(x + half_w, height, posZ + half_d);
}
