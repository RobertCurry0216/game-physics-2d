#include <algorithm>
#include "./Force.h"

Vec2 Force::GenerateDragForce(const Body& body, float k) {
  Vec2 dragForce = Vec2(0.0, 0.0);
  if (body.velocity.MagnitudeSquared() > 0) {
    Vec2 dragDirection = body.velocity.UnitVector() * -1;
    float dragMagnitude = k * body.velocity.MagnitudeSquared();
    dragForce = dragDirection * dragMagnitude;
  }
  return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& body, float k) {
  return body.velocity.UnitVector() * -1 * k;
}

Vec2 Force::GenerateGravitationForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance) {
  Vec2 d = b.position - a.position;

  float distanceSquared = d.MagnitudeSquared();
  //clamp distance for effect in a fake system
  distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

  Vec2 attractionDirection = d.UnitVector();
  float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

  Vec2 attractionForce = attractionDirection * attractionMagnitude;

  return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k) {
  Vec2 d = body.position - anchor;
  float displacement = d.Magnitude() - restLength;

  Vec2 springDirection = d.UnitVector();
  float springMagnitude = -k * displacement;

  Vec2 springForce = springDirection * springMagnitude;
  return springForce;
}

Vec2 Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k) {
  Vec2 d = a.position - b.position;
  float displacement = d.Magnitude() - restLength;

  Vec2 springDirection = d.UnitVector();
  float springMagnitude = -k * displacement;

  Vec2 springForce = springDirection * springMagnitude;
  return springForce;
}