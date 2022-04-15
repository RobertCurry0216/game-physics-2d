#include "CollisionDetection.h"
#include "Body.h"
#include "Vec2.h"
#include "Shape.h"
#include "Contact.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
  bool aIsCircle = a->shape->GetType() == CIRCLE;
  bool bIsCircle = b->shape->GetType() == CIRCLE;

  if (aIsCircle && bIsCircle) {
    return IsCollidingCircleCircle(a, b, contact);
  }

  return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
  CircleShape* aShape = (CircleShape*)a->shape;
  CircleShape* bShape = (CircleShape*)b->shape;

  Vec2 ab = b->position - a->position;
  float sumRadius = aShape->radius + bShape->radius;

  bool isColliding = ab.MagnitudeSquared() <= (sumRadius*sumRadius);

  if (!isColliding) {
    return false;
  }

  // Contact Data
  contact.a = a;
  contact.b = b;
  contact.normal = ab.Normalize();
  contact.start = b->position - (contact.normal * bShape->radius);
  contact.end = a->position + (contact.normal * aShape->radius);
  contact.depth = (contact.end - contact.start).Magnitude();

  return true;
}
