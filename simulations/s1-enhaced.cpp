/*
S1: Implementacion modularizada de simulacion 1
@author: Jeysson4K
*/
#include "lib.h"

int main(int argc, char** argv) {
    bool EXEC = validate_params(2, argc);
    if(!EXEC) return EXIT_FAILURE;

    // Restoring data from config files
    std::map<std::string, float> wc = fetch(argv[1]), bc = fetch(argv[2]);
    W w{}; B b0{};
    init(&w, wc);
    init(&b0, bc);

    // Create world instance
    b2Vec2 gravity(w.gx, w.gy);
    b2World world(gravity);

    // Define and configure ground body 
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(w.x0, w.y0);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(w.width, w.height);
    groundBody->CreateFixture(&groundBox, WORLD_DENSITY);

    b2BallObject* ball0 = new b2BallObject(world, &b0);
   
    /* SimulationHandler handles the entire simulation (for loop) 
    and params can be skipped (because of default values) */
    SimulationHandler* sh = new SimulationHandler();
    // Instance to write in file
    std::ofstream outFile("b0_data.txt");
    sh->simulation_one(world, ball0->ball, outFile, true);
    outFile.close();

    return EXIT_SUCCESS;
}
