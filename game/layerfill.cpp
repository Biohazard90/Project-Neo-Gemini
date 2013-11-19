#include "layerfill.h"

LayerFill::LayerFill(QBrush brush)
{
    this->brush = brush;
}

void LayerFill::OnRender(const render_context_t &context)
{
    context.painter->setBrush(brush);
    context.painter->drawRect(context.x, context.y, context.w, context.h);
}
