
#include "particlebase.h"

#include <QtXml>


class ParticleOpLifetimeDecay : public IParticleOperator
{
public:
    ParticleOpLifetimeDecay()
    {
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpLifetimeDecay(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        if ( particle.data_runtime.lifetime > 0.0f )
        {
            particle.data_runtime.lifetime -= context->GetFrametime();

            if ( particle.data_runtime.lifetime < 0.0f )
                particle.data_runtime.lifetime = 0.0f;
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpLifetimeDecay, oplifetimedecay);
