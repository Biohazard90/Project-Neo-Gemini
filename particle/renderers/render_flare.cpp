
#include "particlebase.h"
#include "render_base.h"

#include <QPainter>


class ParticleRenderFlare : public ParticleRenderBase
{
    std::string material;
    Material *img;
    char materialCount;

    float position;
    float angle_offset;
    bool face_center;
    float alpha_module_min;
    float alpha_module_max;
    float size_module_min;
    float size_module_max;

public:
    ParticleRenderFlare()
    {
        img = NULL;
        materialCount = 0;
        position = 0.0f;
        angle_offset = 0.0f;
        face_center = false;
        alpha_module_min = 1.0f;
        alpha_module_max = 1.0f;
        size_module_min = 1.0f;
        size_module_max = 1.0f;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleRenderFlare(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BASECLASS(ParticleRenderBase);

        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_STRING(material);
            PARTICLE_PARAM_FLOAT(position);
            PARTICLE_PARAM_FLOAT(angle_offset);
            PARTICLE_PARAM_BOOL(face_center);
            PARTICLE_PARAM_FLOAT(alpha_module_min);
            PARTICLE_PARAM_FLOAT(alpha_module_max);
            PARTICLE_PARAM_FLOAT(size_module_min);
            PARTICLE_PARAM_FLOAT(size_module_max);
        PARTICLE_PARAMS_END;
    }

    virtual void DrawParticle(const render_context_t &context, particle_t &particle)
    {
        if (img == NULL)
        {
            img = MaterialPrecache::GetInstance()->GetMaterial(material.c_str());
        }

        float size = VALUE_PROPORTIONAL(particle.data_runtime.size);
        Vector2D origin = particle.data_runtime.origin;

        Camera::GetInstance()->ToScreen(origin);

        Vector2D screenCenter = Camera::GetInstance()->GetScreenMaxs() * 0.5f;
        Vector2D screenDelta = screenCenter - origin;

        float modulateCenter = screenDelta.Length() / (context.w * 0.5f);
        modulateCenter = 1.0f - SATURATE(modulateCenter);

        origin += screenDelta * position;

        float angle = particle.data_runtime.angle;

        if (face_center)
        {
            angle = screenDelta.Angle();
        }

        float alpha = qlerp(modulateCenter, alpha_module_min, alpha_module_max);
        size *= qlerp(modulateCenter, size_module_min, size_module_max);

        float oldOpacity = context.painter->opacity();
        context.painter->setOpacity(oldOpacity
                                    * particle.data_runtime.color[3]
                                    * alpha);

        img->Render(origin, angle + angle_offset,
                    size, context);

        context.painter->setOpacity(oldOpacity);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderFlare, drawflare);

