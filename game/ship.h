#ifndef SHIP_H
#define SHIP_H

#include "gameutil.h"
#include "vector2d.h"

#include "destructible.h"

class Ship : public Destructible
{
    DECLARE_CLASS(Ship, Destructible);
public:
    Ship();

    void Spawn();

    virtual bool IsSolid() const { return true; }

    virtual bool CanShoot() const;
    virtual void SetShootDelay(float delay);

    virtual void Shoot(const Vector2D &direction, float offset = 0.0f);
    virtual void Shoot(const Vector2D &direction, Vector2D offset);
    virtual void Shoot(const Vector2D &direction, const Resource_Projectile_t &projectile, float offset = 0.0f);
    virtual void Shoot(const Vector2D &direction, const Resource_Projectile_t &projectile, Vector2D offset);

protected:
    virtual void SetProjectileType(const Resource_Projectile_t &type);

private:
    Resource_Projectile_t defaultProjectile;

    float shootDelay;
    bool customProjectile;
};

#endif // SHIP_H
