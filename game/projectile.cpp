#include "projectile.h"

#include "gamebase.h"

REGISTER_ENTITY_CLASS(Projectile, projectile);

Projectile::Projectile()
    : playerOwner(false)
{
}

bool Projectile::ShouldCollide(ICollidable *object) const
{
    Entity *entity = (Entity*)object;

    return entity->IsPlayer() && !playerOwner
            || entity->IsEnemy() && playerOwner
            || entity->IsObstacle();
}

void Projectile::OnCollision(ICollidable *other)
{
    Remove();

    if (!data.particle_hit.empty())
        GetGameContext()->GetParticleRoot()->CreateParticles(data.particle_hit.c_str(), GetOrigin(), GetForward());

    Vector2D direction = GetForward();

    Entity *entity = (Entity*)other;
    entity->TakeDamage(1, this, direction);
}

void Projectile::OnRemove()
{
    BaseClass::OnRemove();

    if (!particlesIdle.isNull())
        particlesIdle->StopEmissionAndParticles();
}

void Projectile::Init(const Resource_Projectile_t &data, bool playerOwner)
{
    this->playerOwner = playerOwner;
    this->data = data;

    SetSize(Vector2D(data.size, data.size));

    ParticleSystem *particlesSpawn = NULL;

    if (!data.particle_trail.empty())
        particlesIdle = GetGameContext()->GetParticleRoot()->CreateParticles(data.particle_trail.c_str());
    if (!data.particle_spawn.empty())
        particlesSpawn = GetGameContext()->GetParticleRoot()->CreateParticles(data.particle_spawn.c_str());

    if (!particlesIdle.isNull())
        particlesIdle->SetParticleParent(this);
    if (particlesSpawn != NULL)
        particlesSpawn->SetParticleParent(this);
}

void Projectile::Launch(const Vector2D &direction, const float &volume)
{
    Vector2D directionNormalized(direction);
    directionNormalized.Normalize();
    directionNormalized *= data.speed;

    SetVelocity(directionNormalized);

    if (!data.sound.empty())
        AudioManager::GetInstance()->PlaySound(data.sound.c_str(), volume);
}

void Projectile::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    const Vector2D &worldMins = Camera::GetInstance()->GetWorldMins();
    const Vector2D &worldMaxs = Camera::GetInstance()->GetWorldMaxs();

    if (!IntersectSizeBounds(GetOrigin(), GetSize(),
                             worldMins, worldMaxs))
    {
        Remove();
    }
}
