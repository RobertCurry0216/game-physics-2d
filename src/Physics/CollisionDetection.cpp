#include "CollisionDetection.h"
#include "Body.h"
#include "Vec2.h"
#include "Shape.h"

bool CollisionDetection::IsColliding(Body* a, Body* b) {
  bool aIsCircle = a->shape->GetType() == CIRCLE;
  bool bIsCircle = b->shape->GetType() == CIRCLE;

  if (aIsCircle && bIsCircle) {
    return IsCollidingCircleCircle(a,b);
  }

  return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b) {
  CircleShape* aShape = (CircleShape*)a->shape;
  CircleShape* bShape = (CircleShape*)b->shape;

  const Vec2 ab = b->position - a->position;
  float sumRadius = aShape->radius + bShape->radius;

  bool isColliding = ab.MagnitudeSquared() <= (sumRadius*sumRadius);

  return isColliding;
}
