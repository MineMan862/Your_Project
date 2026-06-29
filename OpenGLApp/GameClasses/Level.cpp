#include "Level.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

Level::Level() {}

void Level::generate() {
  obstacles.clear();
  srand(static_cast<unsigned>(time(nullptr)));
  nextObstacleZ = -20.0f;
}

bool Level::checkCollisions(const Bike &bike) const {
  glm::vec3 bikeMin = bike.getAABBMin();
  glm::vec3 bikeMax = bike.getAABBMax();

  for (const auto &obs : obstacles) {
    if (!obs.active)
      continue;

    if (std::abs(obs.posZ - bike.posZ) > 10.0f)
      continue;

    glm::vec3 obsMin = obs.getAABBMin(laneWidth);
    glm::vec3 obsMax = obs.getAABBMax(laneWidth);

    if (aabbOverlap(bikeMin, bikeMax, obsMin, obsMax)) {
      return true;
    }
  }
  return false;
}

int Level::checkPowerUp(const Bike &bike) {
  for (auto &pu : powerUps) {
    if (!pu.active)
      continue;

    if (bike.targetLane == pu.lane && std::abs(pu.posZ - bike.posZ) < 5.0f) {
      pu.active = false;
      return pu.type;
    }
  }
  return -1;
}

void Level::update(float bikeZ) {
  obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
                                 [bikeZ](const Obstacle &obs) {
                                   return obs.posZ > bikeZ + 50.0f;
                                 }),
                  obstacles.end());
  powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
                                [bikeZ](const PowerUp &pu) {
                                  return pu.posZ > bikeZ + 40.0f || !pu.active;
                                }),
                 powerUps.end());

  static float nextPowerUpZ = -100.0f;
  while (nextPowerUpZ > bikeZ - 250.0f) {
    int laneP = rand() % 3;
    int typeP = rand() % 2;
    powerUps.push_back(PowerUp{laneP, nextPowerUpZ, typeP, true});
    nextPowerUpZ -= (100.0f + rand() % 150);
  }
  while (nextObstacleZ > bikeZ - 250.0f) {
    int lane = rand() % 3;
    ObstacleType type = (rand() % 2 == 0) ? OBSTACLE_LOW : OBSTACLE_TALL;
    obstacles.push_back(Obstacle(lane, nextObstacleZ, type, laneWidth));
    if (obstacleDifficulty > 10 && rand() % 3 == 0) {
      int lane2 = (lane + 1 + rand() % 2) % 3;
      obstacles.push_back(
          Obstacle(lane2, nextObstacleZ, OBSTACLE_TALL, laneWidth));
    }
    float spacing = 15.0f - (obstacleDifficulty * 0.05f);
    if (spacing < 9.0f)
      spacing = 9.0f;

    nextObstacleZ -= spacing;
    obstacleDifficulty++;
  }
}

void Level::reset() { generate(); }

bool Level::aabbOverlap(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB,
                        glm::vec3 maxB) const {
  return (minA.x <= maxB.x && maxA.x >= minB.x) &&
         (minA.y <= maxB.y && maxA.y >= minB.y) &&
         (minA.z <= maxB.z && maxA.z >= minB.z);
}
