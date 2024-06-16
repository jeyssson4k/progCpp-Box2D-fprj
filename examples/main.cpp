#include <stdio.h>
#include <Box2D/Box2D.h>

//g++ -I "ruta al box2d/include" -L "ruta al/box2d/build/bin" main.cpp -lbox2d
int main(int argc, char **argv)
{
    // gravity magnitude 
    const float g = 18.9f;
    const int32 tf = 200;
    // Create gravity vector
    b2Vec2 gravity(0.0f, -g);

    // Create a new world using gravity vector
    b2World world(gravity);

    // Create the ground of the world
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -g); //no hay suelo aun, lo estamos creando
    b2Body* groundBody = world.CreateBody(&groundBodyDef); // Add ground to world

    // Create the polygon of the ground MKS
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.5f, 10.1f);                // Size of rectangle ground
    groundBody->CreateFixture(&groundBox, 0.0f); // The 2nd parameter is the shape density in km/m2.
    // A static body has m=0 by definition

    // create a new object
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;             // it allows to move the object
    bodyDef.position.Set(0.0f, 20.0f);         // init pos
    b2Body* body = world.CreateBody(&bodyDef); // add to world
    b2PolygonShape dynamicBox;                 // create polygon
    dynamicBox.SetAsBox(1.0f, 1.0f);           // size of object

    // set options to the object
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox; // use polygon of object
    fixtureDef.density = 1.5f;
    fixtureDef.friction = 0.5f;
    body->CreateFixture(&fixtureDef); // use settings in object

    float timeStep = 1.0f / 60.0f; // frecuency time of box2d

    int32 velocityIterations = 5; // recommended value: 8
    int32 positionIterations = 2; // recommended value: 3
    // greater values will increase results decreasing performance

    //simulation loop
    for (int32 i = 0; i < tf; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%d\t%4.2f\t%4.2f\t%4.2f\n", i, position.x, position.y, angle);
    }

    return 0;
}
