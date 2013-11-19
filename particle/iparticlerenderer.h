#ifndef IPARTICLERENDERER_H
#define IPARTICLERENDERER_H

#include "particle.h"
#include "iparticlefunction.h"
#include "rendercontext.h"

class QPainter;

class IParticleRenderer : public IParticleFunction
{
public:
    virtual ~IParticleRenderer(){}

    virtual void DrawParticle(const render_context_t &context, particle_t &particle) = 0;
};

#endif // IPARTICLERENDERER_H
