
#include "particlebase.h"

#include <QtXml>


class ParticleInitVelocityRandom : public IParticleInitializer
{
    float velocity_min_x;
    float velocity_min_y;
    float velocity_max_x;
    float velocity_max_y;
    bool localspace;
    bool flipdirection;
    float exponent;

public:
    ParticleInitVelocityRandom()
    {
        velocity_min_x = 0;
        velocity_min_y = 0;
        velocity_max_x = 0;
        velocity_max_y = 0;
        localspace = false;
        flipdirection = false;
        exponent = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleInitVelocityRandom(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(velocity_min_x);
            PARTICLE_PARAM_FLOAT(velocity_min_y);
            PARTICLE_PARAM_FLOAT(velocity_max_x);
            PARTICLE_PARAM_FLOAT(velocity_max_y);
            PARTICLE_PARAM_FLOAT(exponent);
            PARTICLE_PARAM_BOOL(localspace);
            PARTICLE_PARAM_BOOL(flipdirection);
        PARTICLE_PARAMS_END
    }

    virtual void InitializeParticle(IParticleContext *context, particle_t &particle)
    {
        Vector2D vel(qlerp( qPow(qfrand(), exponent), velocity_min_x, velocity_max_x ),
                     qlerp( qPow(qfrand(), exponent), velocity_min_y, velocity_max_y ));

        if (localspace)
        {
            vel.Rotate(context->GetForward());
        }

        if (flipdirection)
        {
            vel.x *= qbrand() ? -1 : 1;
            vel.y *= qbrand() ? -1 : 1;
        }

        particle.data_initial.velocity += vel;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleInitVelocityRandom, initvelocityrandom);
