#ifndef SPRITE_H
#define SPRITE_H

#include "entity.h"

class Material;

class Sprite : public Entity
{
    DECLARE_CLASS(Sprite, Entity);
public:
    Sprite();

    virtual void Spawn();
    virtual void OnRender(const render_context_t &context);

    virtual void SetMaterial(Material *material);
    virtual void SetMaterial(const char *path);

protected:
    virtual float GetOpacity() const;
    virtual void SetOpacity(const float &opacity);
    virtual float GetParticleOpacity() const { return GetOpacity(); }

private:
    Material *material;
    std::string materialPath;

    float opacity;
    bool spawned;
};

#endif // SPRITE_H
