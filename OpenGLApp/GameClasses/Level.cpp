#include "Level.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Level::Level() {}

void Level::generate() {
    obstacles.clear();

    // Seed random
    srand(static_cast<unsigned>(time(nullptr)));

    // Generate ~25 obstacles spread along the level
    float spacing = length / 27.0f; // ~7.4 units between obstacles

    for (int i = 0; i < 25; i++) {
        float z = -(spacing * (i + 1)); // Negative Z (forward)
        int lane = rand() % 3;          // Random lane 0-2

        // Mix of tall and low obstacles
        // More tall at beginning, mix later
        ObstacleType type;
        if (i < 8) {
            type = OBSTACLE_TALL; // First section: only tall (learn lane switching)
        } else if (i < 15) {
            type = (rand() % 3 == 0) ? OBSTACLE_LOW : OBSTACLE_TALL; // Mix
        } else {
            type = (rand() % 2 == 0) ? OBSTACLE_LOW : OBSTACLE_TALL; // More mix
        }

        obstacles.push_back(Obstacle(lane, z, type, laneWidth));

        // Occasionally add a second obstacle on a different lane for extra challenge
        if (i > 10 && rand() % 4 == 0) {
            int lane2 = (lane + 1 + rand() % 2) % 3; // Different lane
            obstacles.push_back(Obstacle(lane2, z, OBSTACLE_TALL, laneWidth));
        }
    }
}

bool Level::checkCollisions(const Bike& bike) const {
    glm::vec3 bikeMin = bike.getAABBMin();
    glm::vec3 bikeMax = bike.getAABBMax();

    for (const auto& obs : obstacles) {
        if (!obs.active) continue;

        // Only check nearby obstacles (optimization)
        if (std::abs(obs.posZ - bike.posZ) > 10.0f) continue;

        glm::vec3 obsMin = obs.getAABBMin(laneWidth);
        glm::vec3 obsMax = obs.getAABBMax(laneWidth);

        if (aabbOverlap(bikeMin, bikeMax, obsMin, obsMax)) {
            return true;
        }
    }
    return false;
}

float Level::getProgress(float bikeZ) const {
    float progress = (-bikeZ) / length;
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    return progress;
}

bool Level::isCompleted(float bikeZ) const {
    return (-bikeZ) >= length;
}

void Level::reset() {
    generate();
}

bool Level::aabbOverlap(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB, glm::vec3 maxB) const {
    return (minA.x <= maxB.x && maxA.x >= minB.x) &&
           (minA.y <= maxB.y && maxA.y >= minB.y) &&
           (minA.z <= maxB.z && maxA.z >= minB.z);
}
