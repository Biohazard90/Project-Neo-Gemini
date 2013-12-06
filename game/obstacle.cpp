#include "obstacle.h"

#include "gamebase.h"

REGISTER_ENTITY_CLASS(Obstacle, obstacle);

Obstacle::Obstacle()
{
}

void Obstacle::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    if ((GetOrigin().x + GetSize().x) < Camera::GetInstance()->GetWorldMins().x)
    {
        Remove();
    }
}

bool Obstacle::ShouldCollide(ICollidable *other) const
{
    Entity *e = (Entity*)other;

    return e->IsEnemy()
            || e->IsPlayer()
            || e->IsProjectile();
}

void Obstacle::OnCollision(ICollidable *other)
{
    Entity *entity = (Entity*)other;

    if (entity->IsPlayer())
    {
        Damage_t damage;
        damage.damage = 1;
        damage.inflictor = this;
        damage.statsInflictorName = GetEntityClassName();
        damage.statsInflictorClass = GetEntityResourceClass();
        entity->TakeDamage(damage);

        Vector2D delta = GetCollisionOrigin() - entity->GetCollisionOrigin();
        TakeDamage(GetHealth(), nullptr, delta);
    }
}

void Obstacle::Init(const Resource_Obstacle_t &data)
{
    BaseClass::Init(data);

    Q_ASSERT(data.materialCount > 0);

    this->data = data;

    if (data.materialCount > 0)
    {
        int index = qrand(data.materialCount - 1);

        Q_ASSERT(index >= 0 && index < data.materialCount);

        SetMaterial(data.materials[index]);
    }

    Vector2D origin = GetOrigin();
    origin.x = Camera::GetInstance()->GetWorldMaxs().x + data.origin_x_offset;
    Teleport(origin);

    SetAngle(qfrand() * 360.0f);
    SetAngularVelocity(qlerp(qfrand(), data.angularvelocity_min, data.angularvelocity_max));

    SetVelocity(Vector2D(qlerp(qfrand(), data.velocity_x_min, data.velocity_x_max), 0));
    float size = qlerp(qfrand(), data.size_min, data.size_max);
    SetSize(Vector2D(size, size));

    SetMaxHealth(qlerp(qfrand(), data.health_min, data.health_max));
}
