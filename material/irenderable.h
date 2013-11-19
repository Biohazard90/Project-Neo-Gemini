#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <QPointF>
#include <QRectF>

#include "rendercontext.h"


class IRenderable
{
public:
    virtual ~IRenderable(){}

    virtual void OnRender(const render_context_t &context) = 0;
};

#endif // IRENDERABLE_H
