
#include "particlebase.h"

#include <QtXml>


class ParticleInitSizeRandom : public IParticleInitializer
{
    float size_min;
    float size_max;
    float exponent;

public:
    ParticleInitSizeRandom()
    {
        size_min = 5;
        size_max = 5;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitSizeRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(size_min);
            PARTICLE_PARAM_FLOAT(size_max);
            PARTICLE_PARAM_FLOAT(exponent);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.size = qlerp(qPow(qfrand(), exponent), size_min, size_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitSizeRandom, initsizerandom);
