
#include "particlebase.h"

#include <QtXml>


class ParticleOpLengthScale : public IParticleOperator
{
    float lifetimefraction_min;
    float lifetimefraction_max;
    float length_min;
    float length_max;
    float exponent;

public:
    ParticleOpLengthScale()
    {
        lifetimefraction_min = 0;
        lifetimefraction_max = 0;
        length_min = 0;
        length_max = 0;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpLengthScale(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(lifetimefraction_min);
            PARTICLE_PARAM_FLOAT(lifetimefraction_max);
            PARTICLE_PARAM_FLOAT(length_min);
            PARTICLE_PARAM_FLOAT(length_max);
            PARTICLE_PARAM_FLOAT(exponent);
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        float fraction = 0.0f;

        if (particle.data_initial.lifetime > 0.0f)
            fraction = 1.0f - particle.data_runtime.lifetime / particle.data_initial.lifetime;

        fraction = qPow(fraction, exponent);

        if (fraction < lifetimefraction_min
                || fraction > lifetimefraction_max)
            return;

        particle.data_runtime.length = particle.data_initial.length *
                                qremapval(fraction,
                                             lifetimefraction_min, lifetimefraction_max,
                                             length_min, length_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpLengthScale, oplengthscale);
