
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

    virtual void Begin(const render_context_t &context)
    {
        context.painter->beginNativePainting();

        glPopAttrib();

//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        glOrtho(0.0, context.w, context.h, 0.0, -1.0, 1.0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        glDepthMask(GL_FALSE);


        glBegin(GL_TRIANGLES);
    }

    virtual void End(const render_context_t &context)
    {
        glEnd();

        glPushAttrib(GL_ENABLE_BIT);

        context.painter->endNativePainting();
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
        //QRectF r(-s.AsQPointF(), s.AsQPointF());

        //context.painter->setBrush(particle.GetColorBrush());
        //context.painter->setPen(particle.GetColorPen());

        //context.painter->translate(origin.AsQPointF());
        //context.painter->drawEllipse(r);

        glColor4f(particle.data_runtime.color[0], particle.data_runtime.color[1],
                particle.data_runtime.color[2], particle.data_runtime.color[3]);

        glVertex2f(origin.x, origin.y - s.y);
        glVertex2f(origin.x + s.x, origin.y);
        glVertex2f(origin.x - s.x, origin.y);

        glVertex2f(origin.x, origin.y + s.y);
        glVertex2f(origin.x - s.x, origin.y);
        glVertex2f(origin.x + s.x, origin.y);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderCircle, drawcircle);
