#include "Level.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

Level::Level() {}

void Level::generate() {
  obstacles.clear();
  srand(static_cast<unsigned>(time(nullptr)));
  nextObstacleZ = -20.0f;
  obstacleDifficulty = 0;
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
  glm::vec3 bikeMin = bike.getAABBMin();
  glm::vec3 bikeMax = bike.getAABBMax();

  for (auto &pu : powerUps) {
    if (!pu.active)
      continue;

    // Use physical AABB check instead of instantaneous targetLane check
    glm::vec3 puCenter = pu.getPosition(laneWidth);
    glm::vec3 puMin = puCenter - glm::vec3(0.8f, 1.0f, 0.8f); // Hitbox del powerup
    glm::vec3 puMax = puCenter + glm::vec3(0.8f, 1.0f, 0.8f);

    if (aabbOverlap(bikeMin, bikeMax, puMin, puMax)) {
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
    ObstacleType type;
    
    float currentDistance = -nextObstacleZ;
    bool spawnSecondObstacle = false;

    if (currentDistance < 500.0f) {
      // Difficoltà Facile: 0 - 500m
      // Solo macerie (OBSTACLE_LOW)
      type = OBSTACLE_LOW;
    } else if (currentDistance < 1000.0f) {
      // Difficoltà Normale: 500 - 1000m
      // Furgoni spawnano meno frequentemente delle macerie (es. 25% probabilità)
      type = (rand() % 4 == 0) ? OBSTACLE_TALL : OBSTACLE_LOW;
    } else {
      // Difficoltà Difficile: > 1000m
      // Spawn bilanciato 50/50
      type = (rand() % 2 == 0) ? OBSTACLE_TALL : OBSTACLE_LOW;
      // Possono spawnare due ostacoli vicini
      if (rand() % 3 == 0) {
        spawnSecondObstacle = true;
      }
    }

    obstacles.push_back(Obstacle(lane, nextObstacleZ, type, laneWidth));
    
    if (spawnSecondObstacle) {
      int lane2 = (lane + 1 + rand() % 2) % 3;
      // Forza il secondo ostacolo ad essere il contrario del primo
      ObstacleType type2 = (type == OBSTACLE_TALL) ? OBSTACLE_LOW : OBSTACLE_TALL;
      obstacles.push_back(Obstacle(lane2, nextObstacleZ, type2, laneWidth));
    }

    float spacing = 18.0f - (currentDistance * 0.003f); // Partenza più larga
    if (spacing < 15.0f)
      spacing = 15.0f; // Limite minimo aumentato di 2m (15.0 invece di 13.0)

    nextObstacleZ -= spacing;
  }
}

void Level::reset() { generate(); }

bool Level::aabbOverlap(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB,
                        glm::vec3 maxB) const {
  return (minA.x <= maxB.x && maxA.x >= minB.x) &&
         (minA.y <= maxB.y && maxA.y >= minB.y) &&
         (minA.z <= maxB.z && maxA.z >= minB.z);
}
