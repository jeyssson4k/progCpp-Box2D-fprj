#define RADIUS 1.0f
#define XCENTER 0.f
#define YCENTER 0.f
#define FRICTION 0.3f
#define RESTITUTION 0.f
#define DENSITY 1.f
#define DEFAULT_POSX 0.f
#define DEFAULT_POSY 25.0f
#define DEFAULT_WIDTH 5.f
#define DEFAULT_HEIGHT 5.f
#define FRAMES 256
#define FPS 1.0f / 60.0f
#define V_ITS 6
#define POS_ITS 2
#define XGRAVITY 0.f
#define YGRAVITY 9.81f
#define WORLD_WIDTH 50.f
#define WORLD_HEIGHT 10.f
#define WORLD_XPOS 0.f
#define WORLD_YPOS 10.f
#define WORLD_DENSITY 0.f
#define SP_X0 0.f
#define SP_Y0 0.f

#define str std::string
#define IParams std::map<str, float>
#define str_array std::vector<str>
#define obj_array std::vector<Obj>

#define b2CreateComponentError "Error creating b2ObjectComponent"
#define BOX2D_COMPONENT_CREATOR_CALL(x)                                                 \
    do                                                                                  \
    {                                                                                   \
        if (x == nullptr)                                                               \
        {                                                                               \
            printf("Error at %s:%d: %s\n", __FILE__, __LINE__, b2CreateComponentError); \
            return EXIT_FAILURE;                                                        \
        }                                                                               \
    } while (0)
