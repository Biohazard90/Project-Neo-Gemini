
#include <qmath.h>

#include "particlebase.h"

class ParticleEmitInstantaneously : public IParticleEmitter
{
    float time;
    float initial_delay_min, initial_delay_max;
    float exponent;
    int amount_min, amount_max;
    bool emit_children;

    bool initialized;

public:
    ParticleEmitInstantaneously()
    {
        time = 0;
        amount_min = amount_max = 10;
        exponent = 1;
        initial_delay_min = initial_delay_max = 0.0f;
        initialized = false;
        emit_children = false;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleEmitInstantaneously(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_INT(amount_min);
            PARTICLE_PARAM_INT(amount_max);
            PARTICLE_PARAM_FLOAT(initial_delay_min);
            PARTICLE_PARAM_FLOAT(initial_delay_max);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_BOOL(emit_children);
        PARTICLE_PARAMS_END
    }

    virtual bool EmitParticles(IParticleContext *context, QList<particle_t*> &particles)
    {
        if (!initialized)
        {
            initialized = true;
            time = qlerp(qfrand(), initial_delay_min, initial_delay_max);
        }

        time -= context->GetFrametime();

        if (time > 0)
            return true;

        int delta = amount_max - amount_min;
        delta = MAX(1, delta);

        int count = qlerp(qPow(qfrand(), exponent) + (0.5f/delta), amount_min, amount_max);

        while (count-- > 0)
        {
            particles.push_back(new particle_t());

            if (emit_children)
            {
                context->EmitChildParticle();
            }
        }

        return false;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleEmitInstantaneously, emitinstantaneously);

