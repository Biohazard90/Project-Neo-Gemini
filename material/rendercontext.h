#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "ipainter.h"

struct render_context_t
{
    //IPainter *painter;
    QPainter *painter;

    int x, y, w, h;

    int mx, my;
};


#endif // RENDERCONTEXT_H
