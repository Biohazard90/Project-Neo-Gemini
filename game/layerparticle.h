#ifndef LAYERPARTICLE_H
#define LAYERPARTICLE_H

#include "layer.h"
#include "vector2d.h"

class ParticleRoot;

class LayerParticle : public Layer
{
public:
    LayerParticle();
    ~LayerParticle();

    virtual void OnRender(const render_context_t &context);
    virtual void OnSimulate(float frametime);

    void AddParticle(const char *name, Vector2D origin);

private:
    ParticleRoot *particleRoot;
};

#endif // LAYERPARTICLE_H
