#include <box2d/box2d.h>

int main(){
    
b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity); 

//Paso 1. 
b2BodyDef groundBodyDef;
groundBodyDef.position.Set(0.0f, -10.0f);

//Paso 2: definición pasa al objeto del mundo para crear el cuerpo tierra. 
b2Body* groundBody = world.CreateBody(&groundBodyDef);

//Paso 3: crear un suelo en forma de polígono. 
b2PolygonShape groundBox;
groundBox.SetAsBox(50.0f, 10.0f);

//Paso 4: pasar la forma y densidad al cuerpo sin necesidad de definir la fixture. 
groundBody->CreateFixture(&groundBox, 0.0f); 

//CREAR UN CUERPO DINÁMICO (ESTABLECIENDO bodyDef.type=b2_dinamicBody PARA QUE SE MUEVA). 
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(0.0f, 4.0f);
b2Body* body = world.CreateBody(&bodyDef);

//crear la forma de caja. 
b2PolygonShape dynamicBox;
dynamicBox.SetAsBox(1.0f, 1.0f);

//definir la fixture usando la caja. 
b2FixtureDef fixtureDef;
fixtureDef.shape = &dynamicBox;
fixtureDef.density = 1.0f;
fixtureDef.friction = 0.3f;

//crear y asociar la fixture al cuerpo. 
body->CreateFixture(&fixtureDef);

//Establecer el paso de tiempo adecuado. 
float timeStep = 1.0f / 60.0f;

//Contador de iteraciones del Constraint solver. 
int32 velocityIterations = 6;
int32 positionIterations = 2;

//Bucle de simulación (60 pasos de tiempo para un total de un segundo)
for (int32 i = 0; i < 60; ++i){

    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

return 0; 
}
