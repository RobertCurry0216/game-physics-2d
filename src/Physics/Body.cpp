#include "Body.h"
#include "Vec2.h"
#include "Shape.h"
#include "../Graphics.h"
#include <iostream>

Body::Body(const Shape& shape, float x, float y, float mass) {
  this->shape = shape.Clone();
  this->position = Vec2(x, y);
  this->mass = mass;
  if (mass != 0.0) {
    this->invMass = 1.0 / mass;
  } else {
    this->invMass = 0.0;
  }
};

Body::~Body() {
  delete shape;
}

void Body::Integrate(float dt) {
  acceleration = sumForces * invMass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Body::AddForce(const Vec2& force) {
  sumForces += force;
}

void Body::ClearForces(void) {
  sumForces = Vec2();
}