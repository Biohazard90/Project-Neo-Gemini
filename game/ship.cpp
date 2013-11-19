#include "ship.h"

#include "gamebase.h"

Ship::Ship()
    : shootDelay(0.0f)
    , customProjectile(false)
{
}

void Ship::SetProjectileType(const Resource_Projectile_t &type)
{
    customProjectile = true;
    defaultProjectile = type;
}

void Ship::Spawn()
{
    BaseClass::Spawn();

    if (!customProjectile)
        defaultProjectile = Resource::GetInstance()->GetProjectile(RESOURCE_PROJECTILE_DEFAULT);
}

bool Ship::CanShoot() const
{
    return shootDelay < GetGameContext()->GetGameTime();
}

void Ship::SetShootDelay(float delay)
{
    shootDelay = GetGameContext()->GetGameTime() + delay;
}

void Ship::Shoot(const Vector2D &direction, float offset)
{
    Shoot(direction, defaultProjectile, offset);
}

void Ship::Shoot(const Vector2D &direction, Vector2D offset)
{
    Shoot(direction, defaultProjectile, offset);
}

void Ship::Shoot(const Vector2D &direction,
                 const Resource_Projectile_t &projectile, float offset)
{
    Shoot(direction, projectile, Vector2D(offset, 0));
}

void Ship::Shoot(const Vector2D &direction,
                 const Resource_Projectile_t &projectile, Vector2D offset)
{
    Projectile *p = (Projectile*)GetGameContext()->CreateEntityNoSpawn("projectile");

    Q_ASSERT(p != NULL);

    float volume = IsPlayer() ? 0.5f : 0.3f;

    offset.Rotate(GetAngle());

    p->SetOwner(this);
    p->Teleport(GetOrigin() + offset);
    p->SetAngle(direction.Angle());
    p->Init(projectile, IsPlayer());
    p->Launch(direction, volume);

    if (projectile.fire_rate > 0.0f)
        shootDelay = GetGameContext()->GetGameTime() + 1.0f / projectile.fire_rate;

    GetGameContext()->SpawnEntity(p);
}
