#ifndef IPARTICLEINITIALIZER_H
#define IPARTICLEINITIALIZER_H

#include "particle.h"
#include "iparticlefunction.h"

class IParticleContext;

class IParticleInitializer : public IParticleFunction
{
public:
    virtual ~IParticleInitializer(){}

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle) = 0;
};

#endif // IPARTICLEINITIALIZER_H
