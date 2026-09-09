#include "Bike.h"
#include <algorithm>

Bike::Bike() { reset(); }

void Bike::moveLeft() {
  if (targetLane > 0) {
    targetLane--;
  }
}

void Bike::moveRight() {
  if (targetLane < 2) {
    targetLane++;
  }
}

void Bike::jump() {
  if (!isJumping) {
    isJumping = true;
    jumpVelocity = jumpForce;
  }
}

void Bike::update(float deltaTime) {
  // Animation
  pedalRotation += 300.0f * deltaTime;

  // Progressive speed logic based on distance
  float dist = -posZ;
  float easySpeed = 18.0f;
  float normalSpeed = 28.0f;

  if (dist < 500.0f) {
    speed = easySpeed;
  } else if (dist < 525.0f) {
    // Smooth transition from easy to normal speed
    float t = (dist - 500.0f) / 25.0f;
    speed = easySpeed + (normalSpeed - easySpeed) * t;
  } else if (dist < 1000.0f) {
    speed = normalSpeed;
  } else {
    // Beyond 1000m, speed is locked but increases by 10% every 500m
    int increments = static_cast<int>((dist - 1000.0f) / 500.0f);
    speed = normalSpeed * std::pow(1.1f, increments);
    if (speed > maxSpeed) speed = maxSpeed;
  }

  // Forward movement (negative Z)
  posZ -= speed * deltaTime;

  // Lateral movement (smooth lane transition)
  float targetX = getXForLane(targetLane);
  float diff = targetX - posX;
  if (std::abs(diff) > 0.01f) {
    float move = laneMoveSpeed * deltaTime;
    if (std::abs(diff) < move) {
      posX = targetX;
      currentLane = targetLane;
    } else {
      posX += (diff > 0 ? 1.0f : -1.0f) * move;
    }
  } else {
    posX = targetX;
    currentLane = targetLane;
  }

  // Jump physics
  if (isJumping) {
    posY += jumpVelocity * deltaTime;
    jumpVelocity -= gravity * deltaTime;
    if (posY <= 0.0f) {
      posY = 0.0f;
      isJumping = false;
      jumpVelocity = 0.0f;
    }
  }
}

void Bike::reset() {
  currentLane = 1;
  targetLane = 1;
  posX = getXForLane(1);
  posY = 0.0f;
  posZ = 0.0f;
  speed = baseSpeed;
  isJumping = false;
  jumpVelocity = 0.0f;
}

float Bike::getXForLane(int lane) const { return (lane - 1) * laneWidth; }

glm::vec3 Bike::getPosition() const { return glm::vec3(posX, posY, posZ); }

glm::vec3 Bike::getAABBMin() const {
  return glm::vec3(posX - halfWidth, posY, posZ - halfDepth);
}

glm::vec3 Bike::getAABBMax() const {
  return glm::vec3(posX + halfWidth, posY + halfHeight * 2.0f,
                   posZ + halfDepth);
}
