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

  Body* box = new Body(BoxShape(200, 100), 400, 400, 1.0);
  bodies.push_back(box);

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
  for (auto Body: bodies) {
    // weight force
    // Vec2 weight = Vec2(0, Body->mass * 9.8 * PIXELS_PER_METER);
    // Body->AddForce(weight);

    // push force
    Body->AddForce(pushForce);

    // mouse
    if (mouseDown) {
      Vec2 towardsMouse = mousePosition - Body->position;
      Body->AddForce(towardsMouse*PIXELS_PER_METER);
    }

    // drag force
    Vec2 drag = Force::GenerateDragForce(*Body, 0.001);
    Body->AddForce(drag);

    //torque
    Body->AddTorque(200);
  }

  for (int i = 0; i < bodies.size(); i++) {
    for (int j = 0; j < bodies.size(); j++) {
      if (i != j) {
        Vec2 springForce = Force::GenerateSpringForce(*bodies[i], *bodies[j], 100, 1000);
        bodies[i]->AddForce(springForce);
      }
    }
  }

  // update bodies
  for (auto body: bodies) {
    body->Update(deltaTime);
  }

  // dirty keep in bounds
  for (auto body: bodies) {
    if (body->position.x < 0 || body->position.x >= Graphics::Width())
    {
      body->velocity.x *= -1;
    }

    if (body->position.y < 0 || body->position.y >= Graphics::Height())
    {
      body->velocity.y *= -1;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
  Graphics::ClearScreen(0xFF222222);

  for (auto body: bodies) {
    if (body->shape->GetType() == CIRCLE) {
      CircleShape* circleShape = (CircleShape*) body->shape;
      Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
    }

    if (body->shape->GetType() == BOX) {
      BoxShape* boxShape = (BoxShape*) body->shape;
      Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVerticies, 0xFFFFFFFF);
    }
  }

  Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
  for (auto body: bodies) {
    delete body;
  }
  Graphics::CloseWindow();
}