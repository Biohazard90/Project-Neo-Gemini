
#include "particlebase.h"

class ParticleEmitContinuously : public IParticleEmitter
{
    float time;
    float initial_delay_min, initial_delay_max;
    float delay_min, delay_max;
    float duration;
    bool emit_children;

    bool initialized;

public:
    ParticleEmitContinuously()
    {
        time = 0;
        duration = 0;
        delay_min = delay_max = 0.1f;
        initial_delay_min = initial_delay_max = 0.0f;
        initialized = false;
        emit_children = false;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleEmitContinuously(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(delay_min);
            PARTICLE_PARAM_FLOAT(delay_max);
            PARTICLE_PARAM_FLOAT(initial_delay_min);
            PARTICLE_PARAM_FLOAT(initial_delay_max);
            PARTICLE_PARAM_FLOAT(duration);
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

        while (time <= 0)
        {
            particles.push_back(new particle_t());

            if (emit_children)
            {
                context->EmitChildParticle();
            }

            float fraction = qfrand();
            time += delay_min + (delay_max - delay_min) * fraction;
        }

        if (duration > 0.0f)
        {
            duration -= context->GetFrametime();

            if (duration <= 0.0f)
                return false;
        }

        return true;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleEmitContinuously, emitcontinuously);

