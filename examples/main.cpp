#include <iostream>
#include <Box2D/Box2D.h>

const float kGravity = 43.5f; // Gravity value in m/s^2

int main() {
    // Define the world
    b2Vec2 gravity(0.0f, -kGravity);
    b2World world(gravity);

    // Create a ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Create a dynamic body
    b2BodyDef dynamicBodyDef;
    dynamicBodyDef.type = b2_dynamicBody;
    dynamicBodyDef.position.Set(0.0f, 4.0f);
    b2Body* dynamicBody = world.CreateBody(&dynamicBodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    dynamicBody->CreateFixture(&fixtureDef);

    // Simulate the world
    for (int32 i = 0; i < 60; ++i) {
        world.Step(1.0f / 60.0f, 6, 2);
        b2Vec2 position = dynamicBody->GetPosition();
        float angle = dynamicBody->GetAngle();
        std::cout << "Position: (" << position.x << ", " << position.y << ") Angle: " << angle << std::endl;
    }

    return 0;
}
