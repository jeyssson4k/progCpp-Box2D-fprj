/*
Utils C++ Library
@author: Jeysson4K
@summary: This library includes Structs, Methods, Handlers and Classes to
optimize simulation flow.
@components: [
  Struct Obj: Describes Component's properties
    (ball, blocks, world, planes);
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

enum b2BodyShape
{
    POLYGON_WORLD,
    CIRCLE,
    POLYGON_BLOCK,
    SLOPED_PLANE
};

/*
 * Parameters for a body
 * including balls, blocks and ground
 */
struct Obj
{
    float gx;
    float gy;
    float x0;
    float y0;
    float width;
    float height;
    float d;
    float r;
    float fr;
    float radius;
    float xcenter;
    float ycenter;

    Obj() : gx(0.f), gy(0.f), x0(0.f), y0(0.f),
            width(0.f), height(0.f), d(0.f), r(0.f), fr(0.f),
            radius(0.f), xcenter(0.f), ycenter(0.f) {};
};

/*
 * Validate if first param exists, else returns second value
 * Similar to ?? (nullish coalescing operator)
 */
template <typename T>
T isNull(T u, T v)
{
    return (&u == NULL) ? v : u;
}

void write(std::ofstream &output, const char *fmt...);
bool validate_params(const int PARAMS, const int argc);
IParams fetch(char *path);
void init(Obj *O, IParams v, b2BodyShape sh);
bool is_valid_path(char *path);
void ObjToString(Obj *O, char *name);
bool validate_paths(const int count, char **argv);
void print_o(obj_array obj, str_array titles);
str str_concat(int n, ...);
class b2ObjectComponent
{
private:
    b2BodyDef objBody;
    void buildBallFixture(Obj *b)
    {
        b2FixtureDef fx;
        b2CircleShape sh;
        sh.m_p.Set(b->xcenter, b->ycenter);
        sh.m_radius = b->radius;
        fx.shape = &sh;
        fx.density = b->d;
        fx.friction = b->fr;
        fx.restitution = b->r;
        this->obj->CreateFixture(&fx);
    }
    void buildWorldFixture(Obj *O)
    {
        b2PolygonShape sh;
        sh.SetAsBox(O->width, O->height);
        this->obj->CreateFixture(&sh, WORLD_DENSITY);
        //printf("World Initialized\n");
    }
    void buildBlockFixture(Obj *O)
    {
        b2FixtureDef fx;
        b2PolygonShape sh;
        sh.SetAsBox(O->width, O->height);
        fx.shape = &sh;
        fx.density = O->d;
        fx.friction = O->fr;
        this->obj->CreateFixture(&fx);
    }
    ~b2ObjectComponent()
    {
        free(this->obj);
        free(&this->objBody);
    }
    void setEdges(char *path)
    {
        float x = 0.f, y = 0.f;
        std::ifstream file(path);
        std::vector<b2Vec2> w;
        while (file >> x >> y)
        {
            b2Vec2 v(x, y);
            w.push_back(v);
        }
        file.close();
        int n = w.size();
        for (int i = 0; i < n; i += 2)
        {
            int h = i + 1;
            b2Vec2 u(0.f, 0.f), v(0.f, 0.f);
            b2EdgeShape e;
            if (h == n)
            {
                printf("setEdges {%d}: The vector is missing, setting default vector (0,0)\n", h);
            }

            // Restoring values
            u.x = isNull(w[i].x, .0f);
            u.y = isNull(w[i].y, .0f);
            v.x = isNull(w[h].x, .0f);
            v.y = isNull(w[h].y, .0f);

            // Setting vectors to edge
            e.SetTwoSided(u, v);
            this->obj->CreateFixture(&e, WORLD_DENSITY);
        }
        return;
    }

public:
    b2Body *obj;

    b2ObjectComponent(b2World &world, Obj *O, b2BodyType bt, b2BodyShape sh, char *path = nullptr)
    {
        this->objBody.type = bt;
        this->objBody.position.Set(O->x0, O->y0);
        this->obj = world.CreateBody(&this->objBody);
        switch (sh)
        {
        case POLYGON_WORLD:
            buildWorldFixture(O);
            break;
        case CIRCLE:
            buildBallFixture(O);
            break;
        case POLYGON_BLOCK:
            buildBlockFixture(O);
            break;
        case SLOPED_PLANE:
            if (path == nullptr)
            {
                printf("Impossible to create Sloped Plane without Edges configuration path. Abort\n");
                this->~b2ObjectComponent();
            }
            else
            {
                this->setEdges(path);
            }
            break;
        default:
            printf("Impossible to create an undefined shape. Abort\n");
            this->~b2ObjectComponent();
        }
       // printf("A new object has been created\n");
    }
};

// Definir una clase que hereda de b2ContactListener
class MyContactListener : public b2ContactListener
{
private:
    float torque;
    b2Vec2 linearImpulse;
    b2Body *ball;
    b2Body *block;

    bool bodyExists(b2Body *b)
    {
        return b == this->ball or b == this->block;
    }

public:
    MyContactListener(float torque, b2Vec2 linearImpulse)
    {
        this->torque = torque;
        this->linearImpulse = linearImpulse;

        //printf("Contact Listener created and registered successfully\n");
        //printf("MyContactListener: { tq: %.5f, ix: %.5f, iy:%.5f }\n", this->torque, this->linearImpulse.x, this->linearImpulse.y);
    }
    void setBall(b2Body *ball)
    {
        this->ball = ball;
    }
    void setCube(b2Body *cube)
    {
        this->block = cube;
    }

    void BeginContact(b2Contact *contact) override
    {
        // Get collision bodies
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();

        // Collision bodies are the same registered by the Listener
        bool bodyA_exists = this->bodyExists(bodyA);
        bool bodyB_exists = this->bodyExists(bodyB);

        if (bodyA_exists and bodyB_exists)
        {
            this->ball->ApplyTorque(this->torque, true);
            this->ball->ApplyLinearImpulseToCenter(this->linearImpulse, true);
            printf("The ball's got a torque and an impulse\n");
        }
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

        //printf("World Controller active\n");
    }
    void Start(b2World &world, b2Body *ball, std::ofstream &output, bool showOutputInConsole)
    {
        write(output, "ssssss", "t", "x", "y", "k", "pe", "r");
        for (int t = 0; t < this->frames; ++t)
        {
            world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
            float r = ball->GetAngle();
            const b2Vec2 g = world.GetGravity();
            const b2Vec2 pos = ball->GetPosition();
            const b2Vec2 v = ball->GetLinearVelocity();
            const float m = ball->GetMass();
            const float k = 0.5 * m * v.y * v.y;
            const float pe = m * std::fabs(g.y) * pos.y;

            write(output, "dfffff", t, pos.x, pos.y, k, pe, r);
            if (showOutputInConsole)
                printf("{t: %d, y: %.10f, vy: %.10f, k: %.10f, pe: %.10f, r: %.10f}\n", t, pos.x, pos.y, k, pe, r);
        }
    }
};
