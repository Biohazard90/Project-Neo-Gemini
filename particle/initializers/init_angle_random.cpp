
#include "particlebase.h"

#include <QtXml>


class ParticleInitAngleRandom : public IParticleInitializer
{
    float angle_min;
    float angle_max;
    float exponent;

public:
    ParticleInitAngleRandom()
    {
        angle_min = 0;
        angle_max = 360;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitAngleRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(angle_min);
            PARTICLE_PARAM_FLOAT(angle_max);
            PARTICLE_PARAM_FLOAT(exponent);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.angle = qlerp(qPow(qfrand(), exponent), angle_min, angle_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitAngleRandom, initanglerandom);

