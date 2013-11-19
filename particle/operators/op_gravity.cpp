
#include "particlebase.h"

#include <QtXml>


class ParticleOpGravity : public IParticleOperator
{
    float gravity_x;
    float gravity_y;
    bool localspace;
    bool indirect;

public:
    ParticleOpGravity()
    {
        gravity_x = 0;
        gravity_y = 0;
        localspace = false;
        indirect = false;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpGravity(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_BOOL(localspace);
            PARTICLE_PARAM_BOOL(indirect);
            PARTICLE_PARAM_FLOAT(gravity_x);
            PARTICLE_PARAM_FLOAT(gravity_y);
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        Vector2D gravity(gravity_x, gravity_y);

        if (localspace)
            gravity.Rotate(context->GetForward());

        if (indirect)
            particle.data_runtime.velocity += gravity * context->GetFrametime();
        else
            particle.data_runtime.origin += gravity * context->GetFrametime();
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpGravity, opgravity);

