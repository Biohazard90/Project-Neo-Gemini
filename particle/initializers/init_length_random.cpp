
#include "particlebase.h"

#include <QtXml>


class ParticleInitLengthRandom : public IParticleInitializer
{
    float length_min;
    float length_max;
    float exponent;

public:
    ParticleInitLengthRandom()
    {
        length_min = 0.1f;
        length_max = 0.1f;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitLengthRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(length_min);
            PARTICLE_PARAM_FLOAT(length_max);
            PARTICLE_PARAM_FLOAT(exponent);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.length = qlerp(qPow(qfrand(), exponent), length_min, length_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitLengthRandom, initlengthrandom);


