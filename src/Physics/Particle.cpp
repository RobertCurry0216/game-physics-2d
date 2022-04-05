#include "Particle.h"
#include "Vec2.h"
#include "../Graphics.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
  this->position = Vec2(x, y);
  this->mass = mass;
  if (mass != 0.0) {
    this->invMass = 1.0 / mass;
  } else {
    this->invMass = 0.0;
  }
};

Particle::~Particle() {
}

void Particle::Integrate(float dt) {
  acceleration = sumForces * invMass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Particle::AddForce(const Vec2& force) {
  sumForces += force;
}

void Particle::ClearForces(void) {
  sumForces = Vec2();
}