#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "entity.h"

class LaserBeam : public Entity
{
    DECLARE_CLASS(LaserBeam, Entity);

public:
    LaserBeam();

    virtual void Init(Entity *owner, const Vector2D &offset, const float &lifetime);

    virtual void Spawn();

    virtual bool IsProjectile() const { return true; }

    virtual bool IsSolid() const { return true; }
    virtual bool ShouldCollide(ICollidable *) const;
    virtual void OnCollision(ICollidable *);

    virtual void OnSimulate(float frametime);
    virtual void OnRender(const render_context_t &context);

private:
    Vector2D offset;
    float lifetime;
    float timer;

    float flashTimer;
    float flashSize;
    float fixedSize;

    bool spawned;

};

#endif // LASERBEAM_H
