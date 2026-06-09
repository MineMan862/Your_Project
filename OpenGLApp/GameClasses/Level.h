#pragma once
#include <vector>
#include "Obstacle.h"
#include "Bike.h"

class Level {
public:
    float length = 200.0f;           // Total level length
    float laneWidth = 3.0f;
    std::vector<Obstacle> obstacles;

    Level();
    void generate();
    bool checkCollisions(const Bike& bike) const;
    float getProgress(float bikeZ) const;
    bool isCompleted(float bikeZ) const;
    void reset();

private:
    bool aabbOverlap(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB, glm::vec3 maxB) const;
};
