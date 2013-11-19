#include "destructible.h"

#include "gamebase.h"

Destructible::Destructible()
{
}

void Destructible::Init(const Resource_Destructible_t &data)
{
    this->data = data;

    SetMaxHealth(data.health);
}

void Destructible::OnDamage(const Damage_t &damage)
{
    Vector2D particleOrigin = GetOrigin();

    if (damage.inflictor != NULL)
        particleOrigin = damage.inflictor->GetCollisionOrigin();

    EmitDamageParticles(data.particle_hit.c_str(), &damage, particleOrigin);

    if (data.sound_hit_count > 0)
    {
        int index = qrand(data.sound_hit_count - 1);
        Q_ASSERT(index >= 0 && index < data.sound_hit_count);
        AudioManager::GetInstance()->PlaySound(data.sound_hit[index].c_str(), qfrand() * 0.2f + 0.3f);
    }
}

void Destructible::OnKilled(const Damage_t *damage)
{
    if (data.score > 0
            && damage != NULL
            && damage->inflictor != NULL)
    {
        if (damage->inflictor->IsProjectile())
        {
            Projectile *projectile = (Projectile*)damage->inflictor;
            if (projectile->IsPlayerOwned())
            {
                Score::GetInstance()->AddPoints(data.score);
            }
        }
    }

    EmitDamageParticles(data.particle_destroy.c_str(), damage, GetOrigin());

    if (data.sound_destroy_count > 0)
    {
        int index = qrand(data.sound_destroy_count - 1);
        Q_ASSERT(index >= 0 && index < data.sound_destroy_count);
        AudioManager::GetInstance()->PlaySound(data.sound_destroy[index].c_str(), qfrand() * 0.3f + 0.2f);
    }
}

void Destructible::EmitDamageParticles(const char *particleName, const Damage_t *damage, Vector2D position)
{
    if (particleName != NULL && *particleName != 0)
    {
        Vector2D direction(GetForward());

        if (damage != NULL)
        {
            direction = damage->direction;
        }

        GetGameContext()->GetParticleRoot()->CreateParticles(particleName,
                                          position, direction);
    }
}
