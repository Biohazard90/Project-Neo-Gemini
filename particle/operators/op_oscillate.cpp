

#include "particlebase.h"

#include <QtXml>


class ParticleOpOscillate : public IParticleOperator
{
    float oscillate_min;
    float oscillate_max;
    float speed;
    float exponent;
    ParticleField_e field;

public:
    ParticleOpOscillate()
    {
        oscillate_min = 0;
        oscillate_max = 1;
        speed = 10;
        exponent = 1;
        field = Field_alpha;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpOscillate(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(oscillate_min);
            PARTICLE_PARAM_FLOAT(oscillate_max);
            PARTICLE_PARAM_FLOAT(speed);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_CUSTOM(field)
            {
                field = StringToParticleField(data);
                return true;
            }
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        float fraction = 0.0f;

        if (particle.data_initial.lifetime > 0.0f)
            fraction = 1.0f - particle.data_runtime.lifetime / particle.data_initial.lifetime;

        float oscillate = sin(particle.seed + fraction * speed) * 0.5f + 0.5f;
        oscillate = qlerp(qPow(oscillate, exponent), oscillate_min, oscillate_max);

        switch (field)
        {
        default:
            DBGWARNING("!! Implement ParticleOpOscillate for field:" << field);
            break;
        case Field_alpha:
            particle.data_runtime.color[3] *= oscillate;
            break;
        case Field_size:
            particle.data_runtime.size *= oscillate;
            break;
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpOscillate, oposcillate);
