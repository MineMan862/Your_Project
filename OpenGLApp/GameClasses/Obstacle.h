#pragma once
#include <glm/glm.hpp>

enum ObstacleType { OBSTACLE_TALL, OBSTACLE_LOW };

class Obstacle {
public:
    int lane;              // 0, 1, 2
    float posZ;            // Z position along track (negative)
    ObstacleType type;

    // Dimensions
    float width = 2.0f;
    float height;
    float depth = 1.0f;

    // Color
    glm::vec3 color;

    bool active = true;

    Obstacle();
    Obstacle(int lane, float posZ, ObstacleType type, float laneWidth);

    glm::vec3 getPosition(float laneWidth) const;
    glm::vec3 getScale() const;

    // AABB
    glm::vec3 getAABBMin(float laneWidth) const;
    glm::vec3 getAABBMax(float laneWidth) const;
};
