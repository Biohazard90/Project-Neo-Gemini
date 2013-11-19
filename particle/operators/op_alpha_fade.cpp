
#include "particlebase.h"

#include <QtXml>


class ParticleOpAlphaFade : public IParticleOperator
{
    float lifetimefraction_min;
    float lifetimefraction_max;
    float alpha_min;
    float alpha_max;
    float exponent;

public:
    ParticleOpAlphaFade()
    {
        lifetimefraction_min = 0;
        lifetimefraction_max = 0;
        alpha_min = 0;
        alpha_max = 0;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpAlphaFade(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(lifetimefraction_min);
            PARTICLE_PARAM_FLOAT(lifetimefraction_max);
            PARTICLE_PARAM_FLOAT(alpha_min);
            PARTICLE_PARAM_FLOAT(alpha_max);
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

        particle.data_runtime.color[3] = particle.data_initial.color[3] *
                                qremapval(fraction,
                                             lifetimefraction_min, lifetimefraction_max,
                                             alpha_min, alpha_max);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpAlphaFade, opalphafade);
