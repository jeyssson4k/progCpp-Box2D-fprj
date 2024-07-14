/*
Utils C++ Library
@author: Jeysson4K
@summary: This library includes Structs, Methods, Handlers and Classes to
optimize simulation flow.
@components: [
  Struct W: Describes World's properties;
  Struct B: Describes Ball's properties;
  isNull(): Verifies if the initial value is not a nullptr;
  write(): Writes a full line into a file;
  fetch(): Gets data from a file ordered by columns (str, float);
  init(): Initializes data from fetch() into Structs;
  b2BallObject: Create new balls;
  SimulationHandler: Controls loop simulation;
]
@const: Include some default values in constants.h (just in case)
*/

#pragma once
#include <Box2D/Box2D.h>
#include <map>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdarg>
#include <fstream>
#include <iomanip>
#include <time.h>
#include "constants.h"

struct W
{
    float gx;
    float gy;
    float x0;
    float y0;
    float width;
    float height;

    W() : gx(0.f), gy(0.f), x0(0.f), y0(0.f), width(0.f), height(0.f){};
};
struct B
{
    float x0;
    float y0;
    float d;
    float r;
    float fr;
    float radius;
    float xcenter;
    float ycenter;

    B() : x0(0.f), y0(0.f), d(0.f), r(0.f), fr(0.f), radius(0.f), xcenter(0.f), ycenter(0.f){};
};

template <typename T>
T isNull(T u, T v)
{
    return (&u == NULL) ? v : u;
}

void write(std::ofstream &output, const char *fmt...);
bool validate_params(const int PARAMS, const int argc);
std::map<std::string, float> fetch(char *path);
void init(W *w, std::map<std::string, float> v);
void init(B *b, std::map<std::string, float> v);

class b2BallObject
{
private:
    b2BodyDef ballDef;
    b2CircleShape dynamicBall;
    b2FixtureDef ballFixture;

public:
    b2Body *ball;
    b2BallObject(b2World &world, B *b)
    {
        this->ballDef.type = b2_dynamicBody;
        this->ballDef.position.Set(b->x0, b->y0);
        this->ball = world.CreateBody(&this->ballDef);
        this->dynamicBall.m_p.Set(b->xcenter, b->ycenter);
        this->dynamicBall.m_radius = b->radius;
        this->ballFixture.shape = &this->dynamicBall;
        this->ballFixture.density = b->d;
        this->ballFixture.friction = b->fr;
        this->ballFixture.restitution = b->r;
        this->ball->CreateFixture(&this->ballFixture);
        printf("A new ball has been created\n");
    }
};

class SimulationHandler
{
private:
    float timeStep;
    int velocityIterations;
    int positionIterations;
    int frames;

public:
    SimulationHandler(int frames = FRAMES, float timeStep = FPS, int v_iterations = V_ITS, int pos_iterations = POS_ITS)
    {
        this->frames = frames;
        this->timeStep = timeStep;
        this->velocityIterations = v_iterations;
        this->positionIterations = pos_iterations;
    }
    void simulation_one(b2World &world, b2Body *ball, std::ofstream &output, bool showOutputInConsole)
    {
        write(output, "sssss", "t", "y", "vy", "k", "pe");
        for (int t = 0; t < this->frames; ++t)
        {
            world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
            const b2Vec2 g = world.GetGravity();
            const b2Vec2 pos = ball->GetPosition();
            const b2Vec2 v = ball->GetLinearVelocity();
            const float m = ball->GetMass();
            const float k = 0.5*m*v.y*v.y;
            const float pe = m*std::fabs(g.y)*pos.y;
            
            write(output, "dffff", t, pos.y, v.y, k, pe);
            if (showOutputInConsole)
                printf("{t: %d, y: %.10f, vy: %.10f, k: %.10f, pe: %.10f}\n", t, pos.y,  v.y, k, pe);
        }
    }
};
