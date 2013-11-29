#include "globals.h"

static Globals _globals;
Globals *pGlobals = &_globals;

Globals::Globals()
{
    curtime = 0.0f;
    frametime = 0.0f;
    antialiasing = true;
    showfps = false;
    musicenabled = true;
    fullscreen = false;

    SetScreen(640, 480);
}

void Globals::SetScreen(int width, int height)
{
    screen_width = width;
    screen_height = height;

    screen_ratio = width/(float)height;
    screen_ratio_inv = 1.0f / screen_ratio;

    screen_center.Init(width / 2, height / 2);
}
