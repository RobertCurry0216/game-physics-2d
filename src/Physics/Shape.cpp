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

float CircleShape::GetMomentOfInertia(float mass) const {
  return 0.5 * radius * radius * mass;
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
  return new PolygonShape(localVerticies);
}

float PolygonShape::GetMomentOfInertia(float mass) const {
  //TODO
  return 1;
}

void PolygonShape::UpdateVerticies(float angle, const Vec2& position) {
  for (int i=0; i<localVerticies.size(); i++) {
    // rotate
    worldVerticies[i] = localVerticies[i].Rotate(angle);

    //translate
    worldVerticies[i] += position;
  }
}


//////////////////////////////////////////////////////
// box
//////////////////////////////////////////////////////
BoxShape::BoxShape(float width, float height) {
  this->width = width;
  this->height = height;

  //create verticies
  localVerticies.push_back(Vec2(-width/2.0, -height/2.0)); // top left
  localVerticies.push_back(Vec2(+width/2.0, -height/2.0)); // top right
  localVerticies.push_back(Vec2(+width/2.0, +height/2.0)); // bottom right
  localVerticies.push_back(Vec2(-width/2.0, +height/2.0)); // bottom left

  worldVerticies.push_back(Vec2(-width/2.0, -height/2.0)); // top left
  worldVerticies.push_back(Vec2(+width/2.0, -height/2.0)); // top right
  worldVerticies.push_back(Vec2(+width/2.0, +height/2.0)); // bottom right
  worldVerticies.push_back(Vec2(-width/2.0, +height/2.0)); // bottom left
}

BoxShape::~BoxShape() {
  
}

ShapeType BoxShape::GetType() const {
  return BOX;
}

Shape* BoxShape::Clone() const {
  return new BoxShape(width, height);
}

float BoxShape::GetMomentOfInertia(float mass) const {
  // 1/12 * (w^2 + h^2)
  return 0.083333 * (width*width + height*height) * mass;
}