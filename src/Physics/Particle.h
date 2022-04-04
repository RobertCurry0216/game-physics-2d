#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;

  int radius = 4;

  float mass;

  Particle(float x, float y, float mass);
  ~Particle();

  void Update(float delta_time);
};

#endif