#include "sprite.h"

#include "gamebase.h"


Sprite::Sprite()
    : material(nullptr)
    , opacity(1)
    , spawned(false)
{
}

void Sprite::Spawn()
{
    BaseClass::Spawn();

    if (!materialPath.empty())
    {
        material = MaterialPrecache::GetInstance()->GetMaterial(materialPath.c_str());
        materialPath.clear();
    }

    spawned = true;
}

void Sprite::OnRender(const render_context_t &context)
{
    BaseClass::OnRender(context);

    if (!IsVisible())
        return;

    if (material != nullptr)
    {
        Vector2D sizeScaled = GetSize();
        Vector2D originAdjusted = GetOrigin();

        Camera::GetInstance()->Scale(sizeScaled);
        Camera::GetInstance()->ToScreen(originAdjusted);

        float oldOpacity = context.painter->opacity();
        context.painter->setOpacity(oldOpacity * GetOpacity());

        material->Render(originAdjusted, GetAngle(), sizeScaled, context);

        context.painter->setOpacity(oldOpacity);
    }
}

void Sprite::SetMaterial(Material *material)
{
    this->material = material;
}

void Sprite::SetMaterial(const char *path)
{
    materialPath = path;

    if (spawned)
    {
        material = MaterialPrecache::GetInstance()->GetMaterial(materialPath.c_str());
    }
}

float Sprite::GetOpacity() const
{
    return opacity;
}

void Sprite::SetOpacity(const float &opacity)
{
    this->opacity = opacity;
}
