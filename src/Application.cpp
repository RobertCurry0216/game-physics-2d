#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include <vector>

bool Application::IsRunning()
{
  return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
  running = Graphics::OpenWindow();

  Particle* ball = new Particle(200, 100, 1.0);
  ball->radius = 4;
  particles.push_back(ball);

  Particle* ball2 = new Particle(300, 100, 1.0);
  ball2->radius = 4;
  particles.push_back(ball2);

  Particle* ball3 = new Particle(300, 200, 1.0);
  ball3->radius = 4;
  particles.push_back(ball3);

  Particle* ball4 = new Particle(200, 200, 1.0);
  ball4->radius = 4;
  particles.push_back(ball4);

  fluid.x = 0;
  fluid.y = Graphics::Height() / 2;
  fluid.w = Graphics::Width();
  fluid.h = Graphics::Height() / 2;
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      if (event.key.keysym.sym == SDLK_UP)
        pushForce.y = -50 * PIXELS_PER_METER;
      if (event.key.keysym.sym == SDLK_DOWN)
        pushForce.y = 50 * PIXELS_PER_METER;
      if (event.key.keysym.sym == SDLK_LEFT)
        pushForce.x = -50 * PIXELS_PER_METER;
      if (event.key.keysym.sym == SDLK_RIGHT)
        pushForce.x = 50 * PIXELS_PER_METER;
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_UP)
        pushForce.y = 0;
      if (event.key.keysym.sym == SDLK_DOWN)
        pushForce.y = 0;
      if (event.key.keysym.sym == SDLK_LEFT)
        pushForce.x = 0;
      if (event.key.keysym.sym == SDLK_RIGHT)
        pushForce.x = 0;
      break;
    case SDL_MOUSEBUTTONDOWN:
      int x, y;
      SDL_GetMouseState(&x, &y);
      mousePosition = Vec2(x, y);
      mouseDown = true;
      break;
    case SDL_MOUSEBUTTONUP:
      mouseDown = false;
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
  static int timePreviousFrame;
  int timeToWait = FRAME_RATE - (SDL_GetTicks() - timePreviousFrame);
  if (timeToWait > 0 && timeToWait <= FRAME_RATE)
  {
    SDL_Delay(timeToWait);
  }
  float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
  if (deltaTime > 0.016)
  {
    deltaTime = 0.016;
  }
  timePreviousFrame = SDL_GetTicks();

  // apply forces
  for (auto particle: particles) {
    // weight force
    Vec2 weight = Vec2(0, particle->mass * 9.8 * PIXELS_PER_METER);
    particle->AddForce(weight);

    // push force
    particle->AddForce(pushForce);

    // mouse
    if (mouseDown) {
      Vec2 towardsMouse = mousePosition - particle->position;
      particle->AddForce(towardsMouse*PIXELS_PER_METER);
    }

    // drag force
    Vec2 drag = Force::GenerateDragForce(*particle, 0.001);
    particle->AddForce(drag);
  }

  for (int i = 0; i < particles.size(); i++) {
    for (int j = 0; j < particles.size(); j++) {
      if (i != j) {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], *particles[j], 100, 1000);
        particles[i]->AddForce(springForce);
      }
    }
  }

  // update particles
  for (auto particle: particles) {
    particle->Integrate(deltaTime);
  }

  // dirty keep in bounds
  for (auto particle: particles) {
    if (particle->position.x < particle->radius || particle->position.x >= Graphics::Width() - particle->radius)
    {
      particle->velocity.x *= -1;
    }

    if (particle->position.y < particle->radius || particle->position.y >= Graphics::Height() - particle->radius)
    {
      particle->velocity.y *= -1;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
  Graphics::ClearScreen(0xFF222222);

  //draw fluid
  // Graphics::DrawFillRect(
  //   fluid.x + fluid.w/2,
  //   fluid.y + fluid.h/2,
  //   fluid.w,
  //   fluid.h,
  //   0xFF6E2713
  // );

  //draw particles
  for (auto particle: particles) {
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius * 2, 0xFFFFFFFF);
  }

  for (int i = 0; i < particles.size(); i++) {
    for (int j = 0; j < particles.size(); j++) {
      if (i != j) {
        Graphics::DrawLine(particles[i]->position.x, particles[i]->position.y, particles[j]->position.x, particles[j]->position.y, 0xFF0000FF);
      }
    }
  }


  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
  for (auto particle: particles) {
    delete particle;
  }
  Graphics::CloseWindow();
}