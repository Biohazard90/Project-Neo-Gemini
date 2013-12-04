
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
        glLineWidth(1.0f);

        glBegin(GL_LINES);
    }

    virtual void End(const render_context_t &context)
    {
        glEnd();

        glPushAttrib(GL_ENABLE_BIT);

        context.painter->endNativePainting();
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

        //QPen pen = particle.GetColorPen();
        //pen.setWidthF(width);

        float endAlpha = 1.0f;

        if (fadeout)
        {
//            QLinearGradient g;
//            g.setColorAt(0, particle.GetColorBrush());
//            g.setColorAt(1.0f, Qt::transparent);
//            g.setStart(vec2_origin.AsQPointF());
//            g.setFinalStop(dir.AsQPointF());
            //pen.setBrush(QBrush(g));

            endAlpha = 0.0f;
        }
        else
        {
            //pen.setBrush(particle.GetColorBrush());
        }

        //context.painter->setPen(pen);
        //context.painter->translate(p0.AsQPointF());
        //context.painter->drawLine(pointf_origin, dir.AsQPointF());


//        glPushMatrix();

//        glTranslatef(p0.x, p0.y, 0);

        glColor4f(particle.data_runtime.color[0], particle.data_runtime.color[1],
                particle.data_runtime.color[2], particle.data_runtime.color[3]);
        glVertex2f(p0.x + pointf_origin.x(), p0.y + pointf_origin.y());

        glColor4f(particle.data_runtime.color[0], particle.data_runtime.color[1],
                particle.data_runtime.color[2], 0.0f);
        glVertex2f(p0.x + dir.x, p0.y + dir.y);

        //glPopMatrix();
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderLine, drawline);

