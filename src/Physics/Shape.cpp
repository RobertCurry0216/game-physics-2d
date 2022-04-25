#include "Shape.h"

#include <limits>

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

Vec2 PolygonShape::EdgeAt(int index) const {
  int currVertex = index;
  int nextVertex = (index + 1) % worldVerticies.size();
  return worldVerticies[nextVertex] - worldVerticies[currVertex];
}

void PolygonShape::UpdateVerticies(float angle, const Vec2& position) {
  for (int i=0; i<localVerticies.size(); i++) {
    // rotate
    worldVerticies[i] = localVerticies[i].Rotate(angle);

    //translate
    worldVerticies[i] += position;
  }
}

float PolygonShape::FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const {
  float separation = std::numeric_limits<float>::lowest();

  // loop all verticies of a
  for (int i=0; i < worldVerticies.size(); i++) {
    // find the normal (perpendicular)
    Vec2 va = worldVerticies[i];
    Vec2 normal = EdgeAt(i).Normal();

    // track minimum seperation
    float minSep = std::numeric_limits<float>::max();
    Vec2 minVertex;

    // loop all verticies of b
    for (int j=0; j < other->worldVerticies.size(); j++) {
      Vec2 vb = other->worldVerticies[j];

      // project vertix of b onto the normal
      float proj = (vb - va).Dot(normal);
      if (proj < minSep) {
        minVertex = vb;
        minSep = proj;
      }
    }

    if (minSep > separation) {
      separation = minSep;
      axis = EdgeAt(i);
      point = minVertex;
    }
  }

  // return the best separation
  return separation;
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