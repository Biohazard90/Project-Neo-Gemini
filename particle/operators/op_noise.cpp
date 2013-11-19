
#include "particlebase.h"

#include <QtXml>


class ParticleOpNoise : public IParticleOperator
{
    int component_out;
    ParticleField_e field;
    float noise_max, noise_min;
    float exponent;

public:
    ParticleOpNoise()
    {
        exponent = 1;
        noise_min = 0;
        noise_max = 1;
        component_out = 0;
        field = Field_alpha;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpNoise(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(noise_min);
            PARTICLE_PARAM_FLOAT(noise_max);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_INT(component_out);
            PARTICLE_PARAM_CUSTOM(field)
            {
                field = StringToParticleField(data);
                return true;
            }
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        Q_ASSERT(component_out >= 0 && component_out <= 1);

        float value = qlerp(qPow(qfrand(), exponent), noise_min, noise_max);

        switch (field)
        {
        default:
            DBGWARNING("!! Implement ParticleOpNoise for field:" << field);
            break;
        case Field_alpha:
            particle.data_runtime.color[3] = value;
            break;
        case Field_size:
            particle.data_runtime.size = value;
            break;
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpNoise, opnoise);

