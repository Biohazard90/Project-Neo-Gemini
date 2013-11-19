
#include "particlebase.h"
#include "render_base.h"

#include <QPainter>

class ParticleRenderLine : public ParticleRenderBase
{
    float length_min;
    float length_max;
    float scale;
    bool fadeout;
public:
    ParticleRenderLine()
    {
        length_min = 0;
        length_max = FLT_MAX;
        scale = 1;
        fadeout = false;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleRenderLine(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BASECLASS(ParticleRenderBase);

        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_BOOL(fadeout);
            PARTICLE_PARAM_FLOAT(length_min);
            PARTICLE_PARAM_FLOAT(length_max);
            PARTICLE_PARAM_FLOAT(scale);
        PARTICLE_PARAMS_END;
    }

    virtual void DrawParticle(const render_context_t &context, particle_t &particle)
    {
        Vector2D dir( particle.data_runtime.velocity );
        float l = dir.Normalize();

        l = CLAMP(l * particle.data_runtime.length, length_min, length_max);

        float width = particle.data_runtime.size * scale;

        if (!IsScreenspaceScale())
        {
            l = VALUE_PROPORTIONAL(l);
            width = VALUE_PROPORTIONAL(width);
        }

        dir *= -l;
        Vector2D p0 = particle.data_runtime.origin;

        if (IsScreenspacePosition())
        {
            VECTOR_PROPORTIONAL_SCREENSPACE(p0);
        }
        else
        {
            Camera::GetInstance()->ToScreen(p0);
        }

        QPen pen = particle.GetColorPen();
        pen.setWidthF(width);

        if (fadeout)
        {
            QLinearGradient g;
            g.setColorAt(0, particle.GetColorBrush());
            g.setColorAt(1.0f, Qt::transparent);
            g.setStart(vec2_origin.AsQPointF());
            g.setFinalStop(dir.AsQPointF());
            pen.setBrush(QBrush(g));
        }
        else
        {
            pen.setBrush(particle.GetColorBrush());
        }

        context.painter->setPen(pen);
        context.painter->translate(p0.AsQPointF());
        context.painter->drawLine(pointf_origin, dir.AsQPointF());
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderLine, drawline);

