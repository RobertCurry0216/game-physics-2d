#include "Particle.h"
#include "Vec2.h"
#include "../Graphics.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
  this->position = Vec2(x, y);
  this->mass = mass;

  std::cout << "Particle constructor called" << std::endl;
};

Particle::~Particle() {
  std::cout << "Particle destructor called" << std::endl;
}

void Particle::Update(float deltaTime) {
  velocity += acceleration * deltaTime;
  position += (velocity * deltaTime);

  if (position.x < radius || position.x >= Graphics::Width() - radius) {
    velocity.x *= -1;
  }

  if (position.y < radius || position.y >= Graphics::Height() - radius) {
    velocity.y *= -1;
  }
}
