#include <iostream>
#include <Box2D/Box2D.h>

// Function to print the position of a body
void PrintPosition(b2Body* body) {
    std::cout << "Body Position: (" << body->GetPosition().x << ", " << body->GetPosition().y << ")" << std::endl;
}

int main() {
    // Set up Box2D world
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    // Create ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    // Simulation loop
    const float timeStep = 1.0f / 60.0f; // 60 Hz
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;

    for (int i = 0; i < 100; ++i) { // Simulate for 100 steps
        world.Step(timeStep, velocityIterations, positionIterations);

        // Print the position of the dynamic body after each step
        PrintPosition(body);
    }

    return 0;
}
