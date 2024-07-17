#include <Box2D/Box2D.h>
#include <iostream>
#include <fstream>

// Definir una clase que hereda de b2ContactListener
class MyContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        
        // Obtener los cuerpos involucrados en la colisión
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // Verificar si uno de los cuerpos es la bola y el otro es el cubo
        if ((bodyA == ball and bodyB == cube) or (bodyA == cube and bodyB == ball)) {
            std::cout << "Colisión detectada entre la bola y el cubo!" << std::endl;
            
            // Aplicar un torque a la bola
            ball->ApplyTorque(10.0f, true);
            
            // Aplicar un impulso. 
            b2Vec2 impulse1(0.0f, 10.0f); 
            ball->ApplyLinearImpulseToCenter(impulse1, true);

            std::cout << "Torque e impulso aplicados a la bola!" << std::endl; 
        }
    }
    
    // Variables para almacenar referencias a la bola y al cubo
    b2Body* ball;
    b2Body* cube;
};

int main() {
    b2Vec2 gravity(5.0f, -9.8f);
    b2World world(gravity);

    // Instanciar y registrar el contact listener
    MyContactListener contactListener;
    world.SetContactListener(&contactListener);

    // Crear el suelo
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    // Crear plano inclinado
    b2BodyDef planeBodyDef;
    planeBodyDef.position.Set(0.0f, 0.0f); 
    b2Body* planeBody = world.CreateBody(&planeBodyDef);

    // Extremos del segmento de línea 1
    b2Vec2 v1(-4.0f, 16.0f);
    b2Vec2 v2(20.0f, 4.0f);
 
    b2EdgeShape edge1;
    edge1.SetTwoSided(v1, v2);

    // Extremos del segmento de línea 2
    b2Vec2 v3(20.0f, 4.0f);
    b2Vec2 v4(28.0f, 0.0f);

    b2EdgeShape edge2;
    edge2.SetTwoSided(v3, v4); 
   
    planeBody->CreateFixture(&edge1, 0.0f);
    planeBody->CreateFixture(&edge2, 0.0f);

    // Crear la bola
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f); 
    b2Body* bodyball = world.CreateBody(&bodyDef);

    b2CircleShape dynamicBall;  
    dynamicBall.m_p.Set(0.0f, 0.0f);
    dynamicBall.m_radius = 1.0f;
 
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBall;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 10.3f;
    fixtureDef.restitution = 1.0f; 

    bodyball->CreateFixture(&fixtureDef);

    //bodyball->SetLinearVelocity(b2Vec2(5.0f, 0.0f));

    // Almacenar referencia a la bola en el contact listener
    contactListener.ball = bodyball;

    // Crear cubo en el plano inclinado
    b2BodyDef cubeBodyDef;
    cubeBodyDef.type = b2_staticBody;
    cubeBodyDef.position.Set(0.0f, 14.0f);
    b2Body* cubeBody = world.CreateBody(&cubeBodyDef);

    b2PolygonShape dynamicCube;
    dynamicCube.SetAsBox(5.0f, 5.0f); 

    b2FixtureDef cubeFixtureDef;
    cubeFixtureDef.shape = &dynamicCube;
    cubeFixtureDef.density = 1.0f;
    cubeFixtureDef.friction = 0.5f;

    cubeBody->CreateFixture(&cubeFixtureDef);

    // Almacenar referencia al cubo en el contact listener
    contactListener.cube = cubeBody;

    std::ofstream outFile("simulation_impulso.txt");
    outFile << "T\tX\tY\tk\tpe\n";

    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 600.0; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations);

        float angle = bodyball->GetAngle();
        const b2Vec2 g = world.GetGravity();
        const b2Vec2 pos = bodyball->GetPosition();
        const b2Vec2 v = bodyball->GetLinearVelocity();
        const float m = bodyball->GetMass();
        const float k = 0.5 * m * v.y * v.y;
        const float pe = m * std::fabs(g.y) * pos.y;

        outFile << i << "\t" << pos.x << "\t" << pos.y << "\t" << k << "\t" << pe << "\n";

        std::cout << "Step " << i << ": (" << pos.x << ", " << pos.y << "), angle: " << angle << std::endl;
    }

    outFile.close();

    return 0;
}
