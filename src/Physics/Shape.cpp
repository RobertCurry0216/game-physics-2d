#include "Shape.h"

//////////////////////////////////////////////////////
// circle
//////////////////////////////////////////////////////
CircleShape::CircleShape(float radius) {
  this->radius = radius;
}

CircleShape::~CircleShape() {

}

ShapeType CircleShape::GetType() const {
  return CIRCLE;
}

Shape* CircleShape::Clone() const {
  return new CircleShape(radius);
}

//////////////////////////////////////////////////////
// Polygon
//////////////////////////////////////////////////////
PolygonShape::PolygonShape(const std::vector<Vec2> verticies) {
  //TODO
}

PolygonShape::~PolygonShape() {
  
}

ShapeType PolygonShape::GetType() const {
  return POLYGON;
}

Shape* PolygonShape::Clone() const {
  return new PolygonShape(verticies);
}


//////////////////////////////////////////////////////
// box
//////////////////////////////////////////////////////
BoxShape::BoxShape(float width, float height) {
  //TODO
}

BoxShape::~BoxShape() {
  
}

ShapeType BoxShape::GetType() const {
  return BOX;
}

Shape* BoxShape::Clone() const {
  return new BoxShape(width, height);
}