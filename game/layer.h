#ifndef LAYER_H
#define LAYER_H

#include "platform_warnings.h"

#include "irenderable.h"
#include "isimulated.h"

#include "platform.h"

class Layer : public IRenderable, ISimulated
{
    Layer(const Layer &other);
public:
    Layer();
    virtual ~Layer();

    virtual void OnSimulate(float frametime){}

};

#endif // LAYER_H
