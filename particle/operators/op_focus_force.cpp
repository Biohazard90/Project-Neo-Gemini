

#include "particlebase.h"

#include <QtXml>


class ParticleOpFocusForce : public IParticleOperator
{
    float scale;

public:
    ParticleOpFocusForce()
    {
        scale = 1.0f;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpFocusForce(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(scale);
        PARTICLE_PARAMS_END
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        Vector2D focusCenter(VALUE_NORMALIZE_X(pGlobals->mouse_x),
                             VALUE_NORMALIZE(pGlobals->mouse_y));
        Vector2D normCenter(NORMAL_SCREEN_WIDTH * 0.5f, NORMAL_SCREEN_HEIGHT * 0.5f);

        focusCenter = (focusCenter - normCenter) * scale + normCenter;

        particle.data_runtime.velocity += Vector2D(particle.data_initial.origin - focusCenter) * context->GetFrametime();
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpFocusForce, opfocusforce);

