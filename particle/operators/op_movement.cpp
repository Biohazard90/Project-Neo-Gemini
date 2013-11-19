

#include "particlebase.h"

#include <QtXml>


class ParticleOpMovement : public IParticleOperator
{
    float drag;
    float angulardrag;

public:
    ParticleOpMovement()
    {
        drag = 0.0f;
        angulardrag = 0.0f;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpMovement(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(drag);
            PARTICLE_PARAM_FLOAT(angulardrag);
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        particle.data_runtime.origin += particle.data_runtime.velocity * context->GetFrametime();
        particle.data_runtime.angle += particle.data_runtime.angularvelocity * context->GetFrametime();

        particle.data_runtime.angle = clamp_angle_positive(particle.data_runtime.angle);

        if (drag != 0.0f)
        {
            float d = 1.0f - MIN( 1.0f, qAbs(drag) *
                                             context->GetFrametime() / DEFAULT_FRAMESPEED_SECF );

            if (drag > 0.0f)
                particle.data_runtime.velocity *= d;
            else if (d != 0.0f)
                particle.data_runtime.velocity /= d;
        }

        if (angulardrag != 0.0f)
        {
            float d = 1.0f - MIN( 1.0f, qAbs(angulardrag) *
                                             context->GetFrametime() / DEFAULT_FRAMESPEED_SECF );

            if (angulardrag > 0.0f)
                particle.data_runtime.angularvelocity *= d;
            else if (d != 0.0f)
                particle.data_runtime.angularvelocity /= d;
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpMovement, opmovement);
