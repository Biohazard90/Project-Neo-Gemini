#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QPointer>

#include "entity.h"
#include "vector2d.h"
#include "gameutil.h"

class ParticleSystem;

class Projectile : public Entity
{
    DECLARE_ENTITY_CLASS(Projectile, Entity);
public:
    Projectile();

    virtual const char *GetEntityResourceClass() { return data.name.c_str(); }

    void Init(const Resource_Projectile_t &data, bool playerOwner = false);

    void Launch(const Vector2D &direction, const float &volume);

    virtual bool IsProjectile() const { return true; }
    virtual bool IsPlayerOwned() const { return playerOwner; }

    virtual bool IsSolid() const { return true; }
    virtual bool ShouldCollide(ICollidable *) const;
    virtual void OnCollision(ICollidable *);

    virtual void OnSimulate(float frametime);
    virtual void OnRemove();

private:
    Resource_Projectile_t data;
    bool playerOwner;

    QPointer<ParticleSystem> particlesIdle;

    QString inflictorName;
    QString inflictorClass;
};

#endif // PROJECTILE_H
