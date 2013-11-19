
#include "particlebase.h"

#include <QtXml>


class ParticleInitSphere : public IParticleInitializer
{
    float distance_min;
    float distance_max;
    float ratio;
    float exponent;
    float velocity_min;
    float velocity_max;
    float velocity_exponent;

public:
    ParticleInitSphere()
    {
        distance_min = 0;
        distance_max = 64;
        ratio = 1;
        velocity_min = 0;
        velocity_max = 0;
        exponent = 1;
        velocity_exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitSphere(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(distance_min);
            PARTICLE_PARAM_FLOAT(distance_max);
            PARTICLE_PARAM_FLOAT(ratio);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_FLOAT(velocity_min);
            PARTICLE_PARAM_FLOAT(velocity_max);
            PARTICLE_PARAM_FLOAT(velocity_exponent);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_initial.origin = context->GetLastOrigin();

        float fraction = qlerp(qPow(qfrand(), exponent), distance_min, distance_max);
        Vector2D dir = Vector2D::AngleDirection(360.0f * qfrand());

        if (ratio != 1.0f)
        {
            dir.x *= ratio;
            dir.y /= ratio;
        }

        particle.data_initial.origin += dir * fraction;

        if (velocity_min != 0.0f || velocity_max != 0.0f)
        {
            particle.data_initial.velocity += dir * qlerp(qPow(qfrand(), velocity_exponent), velocity_min, velocity_max);
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitSphere, initsphere);

