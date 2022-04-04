#include "Application.h"
#include "Physics/Constants.h"

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

  particle = new Particle(50,800,1.0);
  particle->velocity = Vec2(250,-15 * PIXELS_PER_METER);
  particle->acceleration = Vec2(0, 9.8 * PIXELS_PER_METER);
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
  if (timeToWait > 0 && timeToWait <= FRAME_RATE) {
    SDL_Delay(timeToWait);
  }
  float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
  if (deltaTime > 0.016){
    deltaTime = 0.016;
  }
  timePreviousFrame = SDL_GetTicks();

  particle->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
  Graphics::ClearScreen(0xFF222222);
  Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius*2, 0xFFFFFFFF);
  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
  delete particle;

  Graphics::CloseWindow();
}