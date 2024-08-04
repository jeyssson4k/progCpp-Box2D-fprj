#include "lib.h"
int main(int argc, char **argv)
{
    // Amount of params is OK
    bool EXEC = validate_params(5, argc);
    if (!EXEC)
        return EXIT_FAILURE;
    // Paths are OK
    EXEC = validate_paths(5, argv);
    if (!EXEC)
        return EXIT_FAILURE;

    // Restoring data from config files
    IParams wc = fetch(argv[1]), bc = fetch(argv[2]), cc = fetch(argv[3]), config = fetch(argv[4]);
    char *edges_path = argv[5];

    // Structs with component params
    Obj w, b0, c0, sp0;

    // Initializing values
    sp0.x0 = SP_X0;
    sp0.y0 = SP_Y0;
    init(&w, wc, POLYGON_WORLD);
    init(&b0, bc, CIRCLE);
    init(&c0, cc, POLYGON_BLOCK);

    // Create world instance
    b2Vec2 gravity(w.gx, w.gy);
    b2World world(gravity);
    b2Vec2 fi(config["ix"], config["iy"]);
    MyContactListener *contactListener = new MyContactListener(config["tq"], fi);
    world.SetContactListener(contactListener);

    // Print data collected from files
    print_o(obj_array{w, b0, c0, sp0}, str_array{"world", "ball", "cube", "sloped plane"});

    // Trying to create components
    b2ObjectComponent *gr = new b2ObjectComponent(world, &w, b2_staticBody, POLYGON_WORLD);
    BOX2D_COMPONENT_CREATOR_CALL(gr);
    b2ObjectComponent *ball0 = new b2ObjectComponent(world, &b0, b2_dynamicBody, CIRCLE);
    BOX2D_COMPONENT_CREATOR_CALL(ball0);
    b2ObjectComponent *slopedPlane0 = new b2ObjectComponent(world, &sp0, b2_staticBody, SLOPED_PLANE, edges_path);
    BOX2D_COMPONENT_CREATOR_CALL(slopedPlane0);
    b2ObjectComponent *cube0 = new b2ObjectComponent(world, &c0, b2_staticBody, POLYGON_BLOCK);
    BOX2D_COMPONENT_CREATOR_CALL(cube0);

    // Set ball velocity
    ball0->obj->SetLinearVelocity(b2Vec2(config["vx"], config["vy"]));

    // Track objects to collision
    contactListener->setBall(ball0->obj);
    contactListener->setCube(cube0->obj);

    /* SimulationHandler handles the entire simulation (for loop)
    and params can be skipped (because of default values) */
    SimulationHandler *sh = new SimulationHandler((int)config["tf"]);
    // Instance to write in file
    str iy = std::to_string((int)config["iy"]);
    str tq = std::to_string((int)config["tq"]);
    str vx = std::to_string((int)config["vx"]);
    str path = str_concat(8, "s2_", "imp-", iy.c_str(), "_torq-", tq.c_str(), "_vx-", vx.c_str(), "_data.txt");
    printf("Path to save data: %s\n", path.c_str());
    std::ofstream outFile(path);
    // Execution
    sh->Start(world, ball0->obj, outFile, (bool)config["cout"]);
    outFile.close();

    return EXIT_SUCCESS;
}
