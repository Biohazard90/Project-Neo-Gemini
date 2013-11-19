#include "enemyship.h"

#include "gamebase.h"

REGISTER_ENTITY_CLASS(EnemyShip, fighter);

EnemyShip::EnemyShip()
    : aiStrategy(NULL)
{
}

void EnemyShip::Spawn()
{
    BaseClass::Spawn();

    if (aiStrategy != NULL)
        aiStrategy->Spawn();
}

void EnemyShip::OnRemove()
{
    BaseClass::OnRemove();

    if (aiStrategy != NULL)
        aiStrategy->OnRemove();
}

bool EnemyShip::ShouldCollide(ICollidable *object) const
{
    Entity *other = (Entity*)object;

    return BaseClass::ShouldCollide(object)
            || other->IsPlayer()
            || other->IsProjectile();
}

void EnemyShip::OnCollision(ICollidable *other)
{
    Entity *entity = (Entity*)other;

    if (entity->IsPlayer())
    {
        Vector2D delta = other->GetCollisionOrigin() - GetCollisionOrigin();
        delta.Normalize();

        entity->TakeDamage(1, this, delta);
        TakeDamage(GetHealth(), entity, -delta);
    }
}

void EnemyShip::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    if (aiStrategy != NULL)
        aiStrategy->OnSimulate(frametime);

    if (AutoRemoveOffscreen()
            && GetOrigin().x + GetSize().x
            < Camera::GetInstance()->GetWorldMins().x)
        Remove();
}

void EnemyShip::OnMove(float frametime)
{
    BaseClass::OnMove(frametime);

    if (aiStrategy != NULL)
        aiStrategy->OnMove(frametime);
}

void EnemyShip::Init(const Resource_Fighter_t &data, IAI *aiStrategy)
{
    BaseClass::Init(data);

    delete this->aiStrategy;

    this->data = data;

    this->aiStrategy = aiStrategy;

    SetMaterial(data.material);
    SetSize(Vector2D(data.size, data.size));
    SetMaxHealth(data.health);
    SetProjectileType(data.projectile);

    Vector2D origin = GetOrigin();
    origin.x = Camera::GetInstance()->GetWorldMaxs().x + data.size * 0.5f;
    Teleport(origin);

    if (aiStrategy != NULL)
        aiStrategy->Init(this, GetGameContext());
}
