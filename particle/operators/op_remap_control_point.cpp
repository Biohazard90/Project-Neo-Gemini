
#include "particlebase.h"

#include <QtXml>


class ParticleOpRemapControlPoint : public IParticleOperator
{
    int controlpoint;
    int component_in;
    int component_out;
    ParticleField_e field;

public:
    ParticleOpRemapControlPoint()
    {
        controlpoint = 0;
        component_in = 0;
        component_out = 0;
        field = Field_alpha;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpRemapControlPoint(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_INT(controlpoint);
            PARTICLE_PARAM_INT(component_in);
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
        Q_ASSERT(component_in >= 0 && component_in <= 1);
        Q_ASSERT(component_out >= 0 && component_out <= 1);

        const Vector2D &cp = context->GetControlPoint(controlpoint);
        float value = cp[component_in];

        switch (field)
        {
        default:
            DBGWARNING("!! Implement ParticleOpRemapControlPoint for field:" << field);
            break;
        case Field_origin:
            particle.data_runtime.origin[component_out] = value;
            break;
        case Field_alpha:
            particle.data_runtime.color[3] = value;
            break;
        }
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpRemapControlPoint, opremapcontrolpoint);
