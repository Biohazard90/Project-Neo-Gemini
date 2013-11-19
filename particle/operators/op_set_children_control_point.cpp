
#include "particlebase.h"

#include <QtXml>


class ParticleOpSetControlPoint : public IParticleOperator
{
    int controlpoint;
    int component_in;
    int component_out;
    ParticleField_e field;

public:
    ParticleOpSetControlPoint()
    {
        controlpoint = 0;
        component_in = 0;
        component_out = 0;
        field = Field_alpha;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpSetControlPoint(*this);
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

        float value = 0.0f;

        switch (field)
        {
        default:
            DBGWARNING("!! Implement ParticleOpSetControlPoint for field:" << field);
            break;
        case Field_alpha:
            value = particle.data_runtime.color[3];
            break;
        case Field_origin:
            value = particle.data_runtime.origin[component_in];
            break;
        }

        context->SetChildControlPoint(controlpoint, component_out, value);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpSetControlPoint, opsetcontrolpoint);

