#include "lib.h"

int main(int argc, char **argv)
{
    // Amount of params is OK
    bool EXEC = validate_params(2, argc);
    if (!EXEC)
        return EXIT_FAILURE;
    // Paths are OK
    EXEC = validate_paths(2, argv);
    if (!EXEC)
        return EXIT_FAILURE;

    // Restoring data from config files
    IParams wc = fetch(argv[1]), bc = fetch(argv[2]);
    Obj w, b0;
    init(&w, wc, POLYGON_WORLD);
    init(&b0, bc, CIRCLE);

    // Create world instance
    b2Vec2 gravity(w.gx, w.gy);
    b2World world(gravity);

    // Define Components
    b2ObjectComponent *gr = new b2ObjectComponent(world, &w, b2_staticBody, POLYGON_WORLD);
    BOX2D_COMPONENT_CREATOR_CALL(gr);
    b2ObjectComponent *ball0 = new b2ObjectComponent(world, &b0, b2_dynamicBody, CIRCLE);
    BOX2D_COMPONENT_CREATOR_CALL(gr);

    /* SimulationHandler handles the entire simulation (for loop)
    and params can be skipped (because of default values) */
    SimulationHandler *sh = new SimulationHandler(1500);
    // Instance to write in file
    str r = std::to_string(bc["r"]);
    str path = str_concat(3, "s1_r-", r.c_str(), "_data.txt");
    printf("Path to save data: %s\n", path.c_str());
    std::ofstream outFile(path);
    sh->Start(world, ball0->obj, outFile, false);
    outFile.close();

    return EXIT_SUCCESS;
}
