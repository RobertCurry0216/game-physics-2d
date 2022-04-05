#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include "./Physics/Vec2.h"

class Application
{
private:
  bool running = false;
  std::vector<Particle*> particles;
  Vec2 pushForce = Vec2(0.0, 0.0);
  Vec2 mousePosition = Vec2(0.0, 0.0);
  bool mouseDown = false;
  SDL_Rect fluid;

public:
  Application() = default;
  ~Application() = default;
  bool IsRunning();
  void Setup();
  void Input();
  void Update();
  void Render();
  void Destroy();
};

#endif