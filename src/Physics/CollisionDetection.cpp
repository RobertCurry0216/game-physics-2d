
#include "CollisionDetection.h"
#include "Body.h"
#include "Vec2.h"
#include "Shape.h"
#include "Contact.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
  bool aIsCircle = a->shape->GetType() == CIRCLE;
  bool bIsCircle = b->shape->GetType() == CIRCLE;

  bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
  bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

  if (aIsCircle && bIsCircle) {
    return IsCollidingCircleCircle(a, b, contact);
  }

  if (aIsPolygon && bIsPolygon) {
    return IsCollidingPolygonPolygon(a, b, contact);
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



bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
  const PolygonShape* aShape = (PolygonShape*) a->shape;
  const PolygonShape* bShape = (PolygonShape*) b->shape;

  Vec2 axisA, axisB;
  Vec2 pointA, pointB;

  float abSeperation = aShape->FindMinSeparation(bShape, axisA, pointA);
  if (abSeperation >= 0) {
    return false;
  }

  float baSeperation = bShape->FindMinSeparation(aShape, axisB, pointB);
  if (baSeperation >= 0) {
    return false;
  }

  // contact info
  contact.a = a;
  contact.b = b;
  if (abSeperation > baSeperation) {
    contact.depth = -abSeperation;
    contact.normal = axisA.Normal();
    contact.start = pointA;
    contact.end = pointA + contact.normal * contact.depth;
  } else {
    contact.depth = -baSeperation;
    contact.normal = -axisB.Normal();
    contact.end = pointB;
    contact.start = pointB - contact.normal * contact.depth;
  }

  return true;
}