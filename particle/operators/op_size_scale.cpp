

#include "particlebase.h"

#include <QtXml>


class ParticleOpSizeScale : public IParticleOperator
{
    float lifetimefraction_min;
    float lifetimefraction_max;
    float size_min;
    float size_max;
    float exponent;

public:
    ParticleOpSizeScale()
    {
        lifetimefraction_min = 0;
        lifetimefraction_max = 0;
        size_min = 0;
        size_max = 0;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpSizeScale(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(lifetimefraction_min);
            PARTICLE_PARAM_FLOAT(lifetimefraction_max);
            PARTICLE_PARAM_FLOAT(size_min);
            PARTICLE_PARAM_FLOAT(size_max);
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

        particle.data_runtime.size = particle.data_initial.size *
                                qremapval(fraction,
                                             lifetimefraction_min, lifetimefraction_max,
                                             size_min, size_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpSizeScale, opsizescale);
