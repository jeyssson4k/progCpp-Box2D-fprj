#include "lib.h"

/*
 * Write line by line data on file
 * according to a specific format
 * Supports: String(s), float/double (f), int (d)
 * Another data type should be unaccepted by fun
 */
void write(std::ofstream &output, const char *fmt...)
{
    output << std::fixed << std::setprecision(10);
    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0')
    {
        if (*fmt == 's')
        {
            const char *s = va_arg(args, const char *);
            output << s << "\t";
        }
        else if (*fmt == 'f')
        {
            double d = va_arg(args, double);
            output << d << "\t";
        }
        else if (*fmt == 'd')
        {
            double d = va_arg(args, int);
            output << (int)d << "\t";
        }

        ++fmt;
    }
    output << "\n";
    va_end(args);
}

/*
 * Verifies if the expected amount of params
 * was given by the command line
 */
bool validate_params(const int PARAMS, const int argc)
{
    const bool USER_PARAMS = PARAMS + 1 == argc;
    if (!USER_PARAMS)
    {
        printf("ERROR: Insufficient number of parameters for performance.\n\tExpected %d parameters.\n\tGiven %d parameters.\n\n",
               PARAMS, argc - 1);
        return false;
    }
    else
    {
        return true;
    }
}
/*
 * Verifies if given paths are OK
 */
bool validate_paths(const int count, char **argv)
{
    bool exit = true;
    for (int i = 1; i <= count; ++i)
    {
        if (!is_valid_path(argv[i]))
        {
            printf("%s is an invalid path, try again with a valid path. Abort.\n", argv[i]);
            exit = false;
            break;
        }
    }
    return exit;
}

/*
 * Restores data from a file formatted as
 * key\tvalue and converts them into a map
 */
IParams fetch(char *path)
{
    IParams out;
    std::ifstream file(path);
    float val = 0.f;
    std::string key;
    while (file >> key >> val)
    {
        out[key] = val;
        // printf("Saving %.2f into %s\n", val, key.c_str());
    }

    file.close();
    return out;
}

void init(Obj *O, IParams v, b2BodyShape sh)
{
    O->gx = isNull(v["gx"], XGRAVITY);
    O->gy = isNull(v["gy"], -YGRAVITY);
    if (sh == POLYGON_WORLD)
    {
        O->x0 = isNull(v["x0"], WORLD_XPOS);
        O->y0 = isNull(v["y0"], WORLD_YPOS);
        O->width = isNull(v["width"], WORLD_WIDTH);
        O->height = isNull(v["height"], WORLD_HEIGHT);
    }
    else
    {
        O->x0 = isNull(v["x0"], DEFAULT_POSX);
        O->y0 = isNull(v["y0"], DEFAULT_POSY);
        O->width = isNull(v["width"], DEFAULT_WIDTH);
        O->height = isNull(v["height"], DEFAULT_HEIGHT);
    }

    O->d = isNull(v["d"], DENSITY);
    O->r = isNull(v["r"], RESTITUTION);
    O->fr = isNull(v["fr"], FRICTION);
    O->radius = isNull(v["radius"], RADIUS);
    O->xcenter = isNull(v["xcenter"], XCENTER);
    O->ycenter = isNull(v["ycenter"], YCENTER);

    // printf("Struct initialized successfully\n");
}

bool is_valid_path(char *path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return false;
    }
    else
    {
        file.close();
        return true;
    }
}

void ObjToString(Obj *O, str name)
{
    printf("Stats From Object [%s]:\t {\n", name.c_str());
    printf("\tg(%.5f,%.5f),\n", O->gx, O->gy);
    printf("\tr(%.5f,%.5f),\n", O->x0, O->y0);
    printf("\tA(%.5f,%.5f),\n", O->width, O->height);
    printf("\td(%.5f),\n", O->d);
    printf("\tfr(%.5f),\n", O->fr);
    printf("\trestitution(%.5f),\n", O->r);
    printf("\tradius(%.5f),\n", O->r);
    printf("\th,k(%.5f,%.5f),\n", O->xcenter, O->ycenter);
    printf("}\n");
}

void print_o(obj_array obj, str_array titles)
{
    printf("Configuraciones iniciales\n");
    for (int i = 0; i < titles.size(); ++i)
    {
        ObjToString(&obj[i], titles[i]);
    }
}

str str_concat(int n, ...)
{
    str s;
    va_list ptr;
    va_start(ptr, n);
    for (int i = 0; i < n; ++i)
        s.append((str)va_arg(ptr, char *));
    va_end(ptr);

    return s;
}
