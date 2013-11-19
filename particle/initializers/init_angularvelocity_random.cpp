
#include "particlebase.h"

#include <QtXml>


class ParticleInitAngularVelocityRandom : public IParticleInitializer
{
    float angularvelocity_min;
    float angularvelocity_max;
    float exponent;
    bool flipdirection;

public:
    ParticleInitAngularVelocityRandom()
    {
        angularvelocity_min = 0;
        angularvelocity_max = 100;
        exponent = 1;
        flipdirection = true;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitAngularVelocityRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(angularvelocity_min);
            PARTICLE_PARAM_FLOAT(angularvelocity_max);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_BOOL(flipdirection);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        float velocity = qlerp(qPow(qfrand(), exponent), angularvelocity_min, angularvelocity_max);
        if (flipdirection && qbrand())
            velocity *= -1;

        particle.data_initial.angularvelocity = velocity;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitAngularVelocityRandom, initangularvelocityrandom);

