
#include "particlebase.h"
#include "render_base.h"

#include <QPainter>

#define MAX_RANDOM_MATERIALS 8

class ParticleRenderMaterial : public ParticleRenderBase
{
    std::string material[MAX_RANDOM_MATERIALS];
    Material *img[MAX_RANDOM_MATERIALS];
    char materialCount;

public:
    ParticleRenderMaterial()
    {
        memset(img, 0, sizeof(Material*) * MAX_RANDOM_MATERIALS);
        materialCount = 0;
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleRenderMaterial(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        PARTICLE_PARAMS_BASECLASS(ParticleRenderBase);

        PARTICLE_PARAMS_BEGIN
            PARTICLE_PARAM_CUSTOM(material)
            {
                if (materialCount < MAX_RANDOM_MATERIALS)
                {
                    material[materialCount] = data;
                    materialCount++;
                    materialCount = MIN(MAX_RANDOM_MATERIALS, materialCount);
                }
                else
                {
                    DBGWARNING("!! Max random material count too small");
                }
                return true;
            }
        PARTICLE_PARAMS_END;
    }

    virtual void DrawParticle(const render_context_t &context, particle_t &particle)
    {
        if (img[0] == nullptr)
        {
            img[0] = MaterialPrecache::GetInstance()->GetMaterial(material[0].c_str());

            for (int i = 1; i < materialCount; i++)
            {
                if (material[i].length() == 0)
                    break;

                img[i] = MaterialPrecache::GetInstance()->GetMaterial(material[i].c_str());
            }
        }

        Vector2D delta(particle.data_runtime.size, particle.data_runtime.size);

        if (!IsScreenspaceScale())
        {
            Camera::GetInstance()->Scale(delta);
        }

        float size = delta.x * 2.0f;
        Vector2D origin = particle.data_runtime.origin;

        if (IsScreenspacePosition())
        {
            VECTOR_PROPORTIONAL_SCREENSPACE(origin);
        }
        else
        {
            Camera::GetInstance()->ToScreen(origin);
        }

        float oldOpacity = context.painter->opacity();
        context.painter->setOpacity(oldOpacity * particle.data_runtime.color[3]);

        int materialIndex = 0;
        if (materialCount > 1)
        {
            materialIndex = SATURATE((float)particle.seed / (float)USHRT_MAX)
                    * materialCount;
            materialIndex = CLAMP(materialIndex, 0, materialCount - 1);
        }

        img[materialIndex]->Render(origin, particle.data_runtime.angle,
                    size, context);

        context.painter->setOpacity(oldOpacity);
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleRenderMaterial, drawmaterial);
