#include "bossship.h"
#include "gamebase.h"

REGISTER_ENTITY_CLASS(BossShip, boss);

BossShip::BossShip()
    : removalTimer(0.0f)
    , deathFXTimer(0.0f)
{
}

void BossShip::Spawn()
{
    BaseClass::Spawn();

    SetAutoRemoveEnabled(false);
}

bool BossShip::IsSolid() const
{
    if (removalTimer > 0.0f)
        return false;

    return BaseClass::IsSolid();
}

void BossShip::OnCollision(ICollidable *other)
{
    Entity *entity = (Entity*)other;

    if (entity->IsPlayer())
    {
        Vector2D delta = other->GetCollisionOrigin() - GetCollisionOrigin();
        delta.Normalize();

        entity->TakeDamage(1, this, delta);
    }
}

void BossShip::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    const float time = GetGameContext()->GetGameTime();

    if (removalTimer > 0.0f)
    {
        if (removalTimer < time)
        {
            Damage_t damage;
            damage.direction = Vector2D(1, 0);

            BaseClass::OnKilled(&damage);
            Score::GetInstance()->AddPoints(GetFighterData().score);
            Remove();
        }
        else if (deathFXTimer < time)
        {
            deathFXTimer = time + 0.3f + 0.3f * qfrand();

            const char *const deathFXSounds[] = {
                "blast/blast_boss_small_0.wav",
                "blast/blast_boss_small_1.wav",
                "blast/blast_boss_small_2.wav",
            };
            const int deathFXCount = QARRAYSIZE(deathFXSounds);

            AudioManager::GetInstance()->PlaySoundSample(deathFXSounds[qrand(deathFXCount - 1)], 0.2f + qfrand() * 0.2f);

            const char *particleHit = "explosion_boss_0_small"; //GetFighterData().particle_hit.c_str();
            if (particleHit && *particleHit)
            {
                for (int i = 0; i < 3; i++)
                {
                    Vector2D particleOrigin = GetOrigin();

                    particleOrigin.x += GetSize().x * 0.3f * qfrand();
                    particleOrigin.y += GetSize().y * 0.3f * qfrand();

                    GetGameContext()->GetParticleRoot()->CreateParticles(particleHit, particleOrigin, Vector2D::AngleDirection(qfrand() * 360.0f));
                }
            }
        }
        return;
    }

    if (aiStrategy != nullptr)
        aiStrategy->OnSimulate(frametime);
}

void BossShip::OnKilled(const Damage_t *damage)
{
    removalTimer = GetGameContext()->GetGameTime() + 2.5f;
}
