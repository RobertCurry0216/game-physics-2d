#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <vector>

enum ShapeType {
  CIRCLE,
  POLYGON,
  BOX
};

struct Shape {
  virtual ~Shape() = default;
  virtual ShapeType GetType() const = 0;
  virtual Shape* Clone() const = 0;
  virtual float GetMomentOfInertia(float mass) const = 0;
};

struct CircleShape: public Shape {
  float radius;

  CircleShape(const float radius);
  virtual ~CircleShape();
  ShapeType GetType() const override;
  Shape* Clone() const override;
  virtual float GetMomentOfInertia(float mass) const override;
};

struct PolygonShape: public Shape {
  std::vector<Vec2> localVerticies;
  std::vector<Vec2> worldVerticies;

  PolygonShape() = default;
  PolygonShape(const std::vector<Vec2> verticies);
  virtual ~PolygonShape();
  ShapeType GetType() const override;
  Shape* Clone() const override;
  virtual float GetMomentOfInertia(float mass) const override;
  void UpdateVerticies(float angle, const Vec2& position);
};

struct BoxShape: public PolygonShape {
  float width;
  float height;

  BoxShape(float width, float height);
  virtual ~BoxShape();
  ShapeType GetType() const override;
  Shape* Clone() const override;
  virtual float GetMomentOfInertia(float mass) const override;
};

#endif