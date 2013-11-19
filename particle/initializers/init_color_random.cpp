
#include "particlebase.h"

#include <QtXml>


class ParticleInitColorRandom : public IParticleInitializer
{
    float r_min;
    float r_max;
    float g_min;
    float g_max;
    float b_min;
    float b_max;
    float scale;
    float exponent;

public:
    ParticleInitColorRandom()
    {
        r_min = 0;
        r_max = 1;
        g_min = 0;
        g_max = 1;
        b_min = 0;
        b_max = 1;
        scale = 1;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitColorRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_FLOAT(r_min);
            PARTICLE_PARAM_FLOAT(r_max);
            PARTICLE_PARAM_FLOAT(g_min);
            PARTICLE_PARAM_FLOAT(g_max);
            PARTICLE_PARAM_FLOAT(b_min);
            PARTICLE_PARAM_FLOAT(b_max);
            PARTICLE_PARAM_FLOAT(scale);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.color[0] = qlerp(qPow(qfrand(), exponent), r_min, r_max) * scale;
        particle.data_initial.color[1] = qlerp(qPow(qfrand(), exponent), g_min, g_max) * scale;
        particle.data_initial.color[2] = qlerp(qPow(qfrand(), exponent), b_min, b_max) * scale;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitColorRandom, initcolorrandom);
