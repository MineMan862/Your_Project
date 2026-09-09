#pragma once
#include <glm/glm.hpp>

class Bike {
public:
  // Current and target lane
  int currentLane = 1;
  int targetLane = 1;

  // Position
  float posX = 0.0f;
  float posY = 0.0f;
  float posZ = 0.0f;

  // Speed
  float speed = 8.0f;
  float baseSpeed = 8.0f;
  float maxSpeed = 150.0f;
  float speedIncrement = 0.8f;
  
  // Animation
  float pedalRotation = 0.0f;

  // Lane movement
  float laneWidth = 3.0f;
  float laneMoveSpeed = 12.0f;

  // Jump
  bool isJumping = false;
  float jumpVelocity = 0.0f;
  float jumpForce = 8.0f;
  float gravity = 20.0f;

  // Bounding box half-extents
  float halfWidth = 0.4f;
  float halfHeight = 0.75f;
  float halfDepth = 0.75f;

  Bike();
  void moveLeft();
  void moveRight();
  void jump();
  void update(float deltaTime);
  void reset();
  float getXForLane(int lane) const;
  glm::vec3 getPosition() const;

  glm::vec3 getAABBMin() const;
  glm::vec3 getAABBMax() const;
};
