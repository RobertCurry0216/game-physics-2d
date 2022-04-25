#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"
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

  Body* boxA = new Body(BoxShape(200, 200), Graphics::Width()/2, Graphics::Height()/2, 1.0);
  Body* boxB = new Body(BoxShape(200, 200), Graphics::Width()/2, Graphics::Height()/2, 1.0);
  boxA->angularVelocity = 0.4;
  boxB->angularVelocity = 0.1;
  bodies.push_back(boxA);
  bodies.push_back(boxB);

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
    case SDL_MOUSEMOTION:
      int x, y;
      SDL_GetMouseState(&x, &y);
      bodies[0]->position.x = x;
      bodies[0]->position.y = y;
      break;
    // case SDL_MOUSEBUTTONDOWN:
    //   int x, y;
    //   SDL_GetMouseState(&x, &y);
    //   mousePosition = Vec2(x, y);
    //   mouseDown = true;
    //   break;
    // case SDL_MOUSEBUTTONUP:
    //   mouseDown = false;
    //   break;
    // case SDL_MOUSEBUTTONDOWN:
    //   int x, y;
    //   SDL_GetMouseState(&x, &y);
    //   Body* smallBall = new Body(CircleShape(50), x, y, 1.0);
    //   smallBall->restitution = 0.95;
    //   bodies.push_back(smallBall);
    //   break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
  Graphics::ClearScreen(0xFF222222);

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
  for (auto body: bodies) {
    // weight force
    // Vec2 weight = Vec2(0, body->mass * 9.8 * PIXELS_PER_METER);
    // body->AddForce(weight);

    // push force
    //body->AddForce(pushForce);

    // mouse
    // if (mouseDown) {
    //   Vec2 towardsMouse = mousePosition - body->position;
    //   body->AddForce(towardsMouse*PIXELS_PER_METER);
    // }

    // drag force
    // Vec2 drag = Force::GenerateDragForce(*body, 0.001);
    // body->AddForce(drag);

    //torque
    //body->AddTorque(200);
  }

  // for (int i = 0; i < bodies.size(); i++) {
  //   for (int j = 0; j < bodies.size(); j++) {
  //     if (i != j) {
  //       Vec2 springForce = Force::GenerateSpringForce(*bodies[i], *bodies[j], 100, 1000);
  //       bodies[i]->AddForce(springForce);
  //     }
  //   }
  // }

  // update bodies
  for (auto body: bodies) {
    body->Update(deltaTime);
  }

  // collision detection
  for (int i = 0; i < bodies.size() - 1; i++) {
    for (int j = i+1; j < bodies.size(); j++) {
      Body* a = bodies[i];
      Body* b = bodies[j];
      Contact contact;
      a->isColliding = false;
      b->isColliding = false;
      if (CollisionDetection::IsColliding(a, b, contact)) {
        Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
        Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
        Vec2 normalEnd = contact.start + contact.normal * 20;
        Graphics::DrawLine(contact.start.x, contact.start.y, normalEnd.x, normalEnd.y, 0xFFFF00FF);

        // contact.ResolveCollision();
        a->isColliding = true;
        b->isColliding = true;
      }
    }
  }

  // dirty keep in bounds
  for (auto body: bodies) {
    if (body->shape->GetType() == CIRCLE) {
      CircleShape* shape = (CircleShape*)body->shape;
      if (body->position.x < shape->radius)
      {
        body->velocity.x *= -1;
        body->position.x = shape->radius;
      }
      if (body->position.x > Graphics::Width() - shape->radius)
      {
        body->velocity.x *= -1;
        body->position.x = Graphics::Width() - shape->radius;
      }

      if (body->position.y < shape->radius)
      {
        body->velocity.y *= -1;
        body->position.y = shape->radius;
      }
      if (body->position.y > Graphics::Height() - shape->radius)
      {
        body->velocity.y *= -1;
        body->position.y = Graphics::Height() - shape->radius;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{

  for (auto body: bodies) {
    Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
    //Uint32 color = 0xFFFFFFFF;

    if (body->shape->GetType() == CIRCLE) {
      CircleShape* circleShape = (CircleShape*) body->shape;
      Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
    }

    if (body->shape->GetType() == BOX) {
      BoxShape* boxShape = (BoxShape*) body->shape;
      Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVerticies, color);
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