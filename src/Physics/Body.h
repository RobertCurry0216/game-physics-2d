#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body {
  bool isColliding = false;

  //linear motion
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;
  Vec2 sumForces;

  float mass;
  float invMass;

  //angular motion
  float rotation;
  float angularVelocity;
  float angularAcceleration;
  float sumTorque;

  float I;
  float invI;

  //Coefficient of restitution
  float restitution;

  Shape* shape = nullptr;

  Body(const Shape& shape, float x, float y, float mass);
  ~Body();

  bool IsStatic() const;

  void IntegrateLinear(float dt);
  void AddForce(const Vec2& force);
  void ClearForces(void);

  void IntegrateAngular(float dt);
  void AddTorque(float torque);
  void ClearTorque(void);

  void Update(float dt);

  void ApplyImpulse(const Vec2& j);
};

#endif