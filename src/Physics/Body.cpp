#include "Body.h"
#include "Vec2.h"
#include "Shape.h"
#include "../Graphics.h"
#include <iostream>

Body::Body(const Shape& shape, float x, float y, float mass) {
  this->shape = shape.Clone();
  this->position = Vec2(x, y);
  this->velocity = Vec2(0,0);
  this->acceleration = Vec2(0,0);
  this->rotation = 0.0;
  this->angularAcceleration = 0.0;
  this->angularVelocity = 0.0;
  this->sumForces = Vec2(0,0);
  this->sumTorque = 0.0;
  this->mass = mass;
  if (mass != 0.0) {
    this->invMass = 1.0 / mass;
  } else {
    this->invMass = 0.0;
  }
  I = shape.GetMomentOfInertia(mass);
  if (I != 0.0) {
    this->invI = 1.0 / I;
  } else {
    this->invI = 0.0;
  }
};

Body::~Body() {
  delete shape;
}

void Body::IntegrateLinear(float dt) {
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

void Body::IntegrateAngular(float dt) {
  angularAcceleration = sumTorque * invI;
  angularVelocity += angularAcceleration * dt;
  rotation += angularVelocity * dt;

  ClearTorque();
}

void Body::AddTorque(float torque) {
  sumTorque += torque;
}

void Body::ClearTorque(void) {
  sumTorque = 0.0;
}

void Body::Update(float dt) {
  IntegrateLinear(dt);
  IntegrateAngular(dt);
  bool isPolygon = shape->GetType() == POLYGON || shape->GetType() == BOX;
  if (isPolygon) {
    PolygonShape* polygon = (PolygonShape*) shape;
    polygon->UpdateVerticies(rotation, position);
  }
}