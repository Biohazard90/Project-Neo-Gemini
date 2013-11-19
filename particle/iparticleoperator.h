#ifndef IPARTICLEOPERATOR_H
#define IPARTICLEOPERATOR_H

#include "particle.h"
#include "iparticlefunction.h"

class IParticleContext;

class IParticleOperator : public IParticleFunction
{
public:
    virtual ~IParticleOperator(){}

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle) = 0;
};

#endif // IPARTICLEOPERATOR_H
