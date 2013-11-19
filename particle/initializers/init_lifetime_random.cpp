
#include "particlebase.h"

#include <QtXml>


class ParticleInitLifetimeRandom : public IParticleInitializer
{
    float lifetime_min;
    float lifetime_max;
    float exponent;

public:
    ParticleInitLifetimeRandom()
    {
        lifetime_min = 5;
        lifetime_max = 5;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitLifetimeRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(lifetime_min);
            PARTICLE_PARAM_FLOAT(lifetime_max);
            PARTICLE_PARAM_FLOAT(exponent);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.lifetime = qlerp(qPow(qfrand(), exponent), lifetime_min, lifetime_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitLifetimeRandom, initlifetimerandom);

