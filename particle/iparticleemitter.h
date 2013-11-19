#ifndef IPARTICLEEMITTER_H
#define IPARTICLEEMITTER_H

#include "particle.h"
#include "iparticlefunction.h"

class IParticleContext;

class IParticleEmitter : public IParticleFunction
{
public:
    virtual ~IParticleEmitter(){}

    virtual bool EmitParticles(IParticleContext *context, QList<particle_t*> &particles) = 0;
};

#endif // IPARTICLEEMITTER_H
