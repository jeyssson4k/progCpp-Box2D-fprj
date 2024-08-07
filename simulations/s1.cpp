/*
S1: Simulacion de una bola rebotando con restitucion 0
@author: Liana
*/

#include <Box2D/Box2D.h>
#include <iostream>
#include <fstream>

int main() {
    // Crear el vector de gravedad
    b2Vec2 gravity(0.0f, -9.8f);

    // Crear el mundo con el vector de gravedad
    b2World world(gravity);

    // Definir el cuerpo del suelo
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Crear la forma del suelo (caja)
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    // Añadir la forma del suelo al cuerpo del suelo
    groundBody->CreateFixture(&groundBox, 0.0f); //el segundo parametro es de densidad 

    // Definir el cuerpo dinámico (una bola)
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f); // Altura inicial de la bola
    b2Body* body = world.CreateBody(&bodyDef);

    // Crear la forma de la bola (círculo)
    b2CircleShape dynamicCircle;  //b2CircleShape es una clase de Box2D que representa una forma circular
    dynamicCircle.m_p.Set(0.0f, 0.0f); //m_p es un miembro de b2CircleShape que representa la posición del centro del círculo en coordenadas locales.
    dynamicCircle.m_radius = 1.0f;

    // Definir las propiedades físicas de la bola
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f; // Coeficiente de restitución (rebote)

    // Añadir la forma de la bola al cuerpo dinámico
    body->CreateFixture(&fixtureDef);

    // Archivo para guardar los datos de la simulación
    std::ofstream outFile("simulation_data.txt");
    outFile << "TimeStep\tPosX\tPosY\tAngle\n";

    // Simular el mundo durante 60 segundos (3600 pasos de tiempo)
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 1060; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations);

        // Obtener la posición y el ángulo del cuerpo
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

        // Guardar los datos en el archivo
        outFile << i << "\t" << position.x << "\t" << position.y << "\t" << angle << "\n";

        // Imprimir en consola (opcional)
        std::cout << "Step " << i << ": (" << position.x << ", " << position.y << "), angle: " << angle << std::endl;
    }

    outFile.close();

    return 0;
}
