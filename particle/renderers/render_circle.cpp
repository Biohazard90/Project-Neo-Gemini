
#include "particlebase.h"
#include "render_base.h"

#include <QPainter>

class ParticleRenderCircle : public ParticleRenderBase
{
    float scale;
public:
    ParticleRenderCircle()
    {
        scale = 1;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleRenderCircle(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BASECLASS(ParticleRenderBase);

        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_FLOAT(scale);
        PARTICLE_PARAMS_END;
    }

    virtual void DrawParticle(const render_context_t &context, particle_t &particle)
    {
        Vector2D s(particle.data_runtime.size, particle.data_runtime.size);
        Vector2D origin = particle.data_runtime.origin;

        if (IsScreenspacePosition())
        {
            VECTOR_PROPORTIONAL_SCREENSPACE(origin);
        }
        else
        {
            Camera::GetInstance()->ToScreen(origin);
        }

        if (!IsScreenspaceScale())
        {
            s.x = VALUE_PROPORTIONAL(s.x);
            s.y = VALUE_PROPORTIONAL(s.y);
        }

        s *= scale;
        QRectF r(-s.AsQPointF(), s.AsQPointF());

        context.painter->setBrush(particle.GetColorBrush());
        context.painter->setPen(particle.GetColorPen());

        context.painter->translate(origin.AsQPointF());
        context.painter->drawEllipse(r);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderCircle, drawcircle);
