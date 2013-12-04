#ifndef GLOBALS_H
#define GLOBALS_H

#include "vector2d.h"

class Globals
{
public:
    Globals();

    void SetScreen(int width, int height);

    float curtime;
    float frametime;
    float fps;

    int screen_width;
    int screen_height;
    float screen_ratio;
    float screen_ratio_inv;
    Vector2D screen_center;

    bool antialiasing;
    bool showfps;
    bool musicenabled;
    bool fullscreen;
};

extern Globals *pGlobals;

#endif // GLOBALS_H
