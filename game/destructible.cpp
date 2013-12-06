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

    if (damage.inflictor != nullptr)
        particleOrigin = damage.inflictor->GetCollisionOrigin();

    EmitDamageParticles(data.particle_hit.c_str(), &damage, particleOrigin);

    if (data.sound_hit_count > 0)
    {
        int index = qrand(data.sound_hit_count - 1);
        Q_ASSERT(index >= 0 && index < data.sound_hit_count);
        AudioManager::GetInstance()->PlaySoundSample(data.sound_hit[index].c_str(), qfrand() * 0.2f + 0.3f);
    }
}

void Destructible::OnKilled(const Damage_t *damage)
{
    if (data.score > 0
            && damage != nullptr
            && damage->inflictor != nullptr)
    {
        if (damage->inflictor->IsProjectile())
        {
            Projectile *projectile = (Projectile*)damage->inflictor;
            if (projectile->IsPlayerOwned())
            {
                Score::GetInstance()->AddPoints(data.score);

                SendDestroyedEvent();
            }
        }
    }

    EmitDamageParticles(data.particle_destroy.c_str(), damage, GetOrigin());

    if (data.sound_destroy_count > 0)
    {
        int index = qrand(data.sound_destroy_count - 1);
        Q_ASSERT(index >= 0 && index < data.sound_destroy_count);
        AudioManager::GetInstance()->PlaySoundSample(data.sound_destroy[index].c_str(), qfrand() * 0.3f + 0.2f);
    }
}

void Destructible::SendDestroyedEvent()
{
    KeyValues *event = new KeyValues("object_destroyed");
    event->SetString("object_name", GetEntityClassName());
    event->SetString("object_class", GetEntityResourceClass());
    event->SetInt("score", data.score);
    event->SetFloat("x", GetOrigin().x);
    event->SetFloat("y", GetOrigin().y);
    Events::GetInstance()->FireEvent(event);
}

void Destructible::EmitDamageParticles(const char *particleName, const Damage_t *damage, Vector2D position)
{
    if (particleName != nullptr && *particleName != 0)
    {
        Vector2D direction(GetForward());

        if (damage != nullptr)
        {
            direction = damage->direction;
        }

        GetGameContext()->GetParticleRoot()->CreateParticles(particleName,
                                          position, direction);
    }
}
