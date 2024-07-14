/*
Implementations of lib.h
@author: Jeysson4K
*/

#include "lib.h"

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
            output << d << "\t";
        }
        
        ++fmt;
    }
    output << "\n";
    va_end(args);
}

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

std::map<std::string, float> fetch(char *path)
{
    std::map<std::string, float> out;
    std::ifstream file(path);
    float val = 0.f;
    std::string key;
    while (file >> key >> val)
    {
        out[key] = val;
        //printf("Saving %.2f into %s\n", val, key.c_str());
    }

    file.close();
    return out;
}

void init(W *w, std::map<std::string, float> v)
{
    w->gx = isNull(v["gx"], XGRAVITY);
    w->gy = isNull(v["gy"], -YGRAVITY);
    w->x0 = isNull(v["x0"], WORLD_XPOS);
    w->y0 = isNull(v["y0"], WORLD_YPOS);
    w->width = isNull(v["width"], WORLD_WIDTH);
    w->height = isNull(v["height"], WORLD_HEIGHT);
}

void init(B *b, std::map<std::string, float> v)
{
    b->x0 = isNull(v["x0"], DEFAULT_POSX);
    b->y0 = isNull(v["y0"], DEFAULT_POSY);
    b->d = isNull(v["d"], DENSITY);
    b->r = isNull(v["r"], RESTITUTION);
    b->fr = isNull(v["fr"], FRICTION);
    b->radius = isNull(v["radius"], RADIUS);
    b->xcenter = isNull(v["xcenter"], XCENTER);
    b->ycenter = isNull(v["ycenter"], YCENTER);
}
