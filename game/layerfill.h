#ifndef LAYERFILL_H
#define LAYERFILL_H

#include "layer.h"

#include <QBrush>

class LayerFill : public Layer
{
public:
    LayerFill(QBrush brush);

    virtual void OnRender(const render_context_t &context);

private:
    QBrush brush;
};

#endif // LAYERFILL_H
