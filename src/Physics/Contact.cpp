#include "Contact.h"
#include "Vec2.h"
#include <math.h>

void Contact::ResolvePenetration(void) {
  if (a->IsStatic() && b->IsStatic()) {
    return;
  }
  float da = depth / (a->invMass + b->invMass) * a->invMass;
  float db = depth / (a->invMass + b->invMass) * b->invMass;

  a->position -= normal * da;
  b->position += normal * db;
}

void Contact::ResolveCollision(void) {
  ResolvePenetration();

  float elasticity = std::min(a->restitution, b->restitution);

  const Vec2 vrel = a->velocity - b->velocity;

  float vrelDotNormal = vrel.Dot(normal);

  const Vec2 impulseDirection = normal;
  const float impuldeMagnitude = -(1+elasticity) * vrelDotNormal / (a->invMass + b->invMass);

  Vec2 j = impulseDirection * impuldeMagnitude;

  a->ApplyImpulse(j);
  b->ApplyImpulse(-j);
}