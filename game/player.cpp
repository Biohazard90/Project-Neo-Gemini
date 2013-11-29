#include "player.h"

#include "gamebase.h"

#define SPAWN_DURATION 2.0f
#define RESPAWN_DELAY 4.0f
#define IMMUNITY_DURATION 4.0f
#define PLAYER_AXIS_SPEED 200.0f
#define PLAYER_SIZE_X 85.0f
#define SHIELD_FADE_DURATION 7.0f
#define SHIELD_DOWN_DURATION (C_PI_F * 6.0f)
#define SHIELD_DOWN_DELAY 5.0f
#define SHIELD_DOWN_SPEED 11.0f

REGISTER_ENTITY_CLASS(Player, player);

Player::Player()
    : shieldMaterial(nullptr)
    , shieldDownMaterial(nullptr)
    , shieldDownTime(0.0f)
{
    spawnTimer = 0;
    immunityTimer = 0;
    spawnFinished = false;
    respawnDelay = 0.0f;
    shieldTime = 0.0f;

    SetAutoRemoveEnabled(false);

    SetPlayerSprite("player/player_0");

    SetSize(Vector2D(PLAYER_SIZE_X, PLAYER_SIZE_X * 0.666666f));
    SetOrigin(Vector2D(0, 0));
    SetAngle(0);
}

void Player::Spawn()
{
    BaseClass::Spawn();

    SetHealth(4);

    ResetSpawnAnimation();
}

bool Player::IsVisible() const
{
    if (respawnDelay > 0.0f)
        return false;

    return BaseClass::IsVisible();
}

bool Player::IsSolid() const
{
    if (IsImmunityActive())
        return false;

    if (!IsVisible())
        return false;

    if (shieldTime > 0.0f)
        return false;

    return BaseClass::IsSolid();
}

bool Player::ShouldCollide(ICollidable *object) const
{
    Entity *other = (Entity*)object;

    return BaseClass::ShouldCollide(object)
            || other->IsEnemy()
            || other->IsProjectile()
            || other->IsObstacle();
}

void Player::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    if (HandleRespawn())
        return;

    if (HandleSpawn())
        return;

    UpdateMove(frametime);

    UpdateSway(frametime);

    UpdateShoot(frametime);

    shieldTime = qapproachlinear(0.0f, shieldTime, frametime * 12.0f);
    shieldDownTime = qapproachlinear(0.0f, shieldDownTime, frametime * SHIELD_DOWN_SPEED);
}

void Player::OnMove(float frametime)
{
    if (IsSpawning())
    {
        SetVelocity(vec2_origin);

        float fraction = 1.0f - (spawnTimer - GetGameContext()->GetGameTime()) / SPAWN_DURATION;
        Vector2D move;

        move.x = sin(fraction * C_PI * 0.65f) * 320.0f - 120.0f;

        SetOrigin(move);
        return;
    }

    BaseClass::OnMove(frametime);

    Vector2D origin = GetOrigin();
    Vector2D sizeHalf = GetSize() * 0.5f;

    Vector2D worldMins = Camera::GetInstance()->GetWorldMins();
    Vector2D worldMaxs = Camera::GetInstance()->GetWorldMaxs();

    origin.x = CLAMP(origin.x, worldMins.x + sizeHalf.x, worldMaxs.x - sizeHalf.x);
    origin.y = CLAMP(origin.y, worldMins.y + sizeHalf.y, worldMaxs.y - sizeHalf.y);
    SetOrigin(origin);
}

bool Player::HandleSpawn()
{
    const bool spawning = IsSpawning();

    if (!spawnFinished && !spawning)
    {
        SetVelocity(Vector2D(-200, 0));
        CreateParticles();

        spawnFinished = true;
        return true;
    }

    return spawning;
}

bool Player::HandleRespawn()
{
    if (respawnDelay > 0.0f)
    {
        if (respawnDelay < GetGameContext()->GetGameTime())
        {
            respawnDelay = 0.0f;
            Spawn();
        }

        return true;
    }

    return false;
}

void Player::UpdateMove(float frametime)
{
    const float playerSpeed = PLAYER_AXIS_SPEED;
    Vector2D velocity;

    if (Input::GetInstance()->IsButtonDown(BT_UP))
        velocity.y -= playerSpeed;

    if (Input::GetInstance()->IsButtonDown(BT_DOWN))
        velocity.y += playerSpeed;

    if (Input::GetInstance()->IsButtonDown(BT_RIGHT))
        velocity.x += playerSpeed;

    if (Input::GetInstance()->IsButtonDown(BT_LEFT))
        velocity.x -= playerSpeed;

    Vector2D lastVelocity(GetVelocity());
    Vector2D delta = velocity - lastVelocity;

    float acceleration = 20.0f;
    if (velocity.IsZero())
        acceleration = 10.0f;

    float length = delta.Normalize();
    float speed = length * frametime * acceleration;
    speed = MIN(speed, length);

    lastVelocity += delta * speed;

    SetVelocity(lastVelocity);
}

void Player::UpdateSway(float frametime)
{
    float currentAngle = GetAngle();
    float desiredAngle = QREMAPVAL(GetVelocity().y,
                                          -PLAYER_AXIS_SPEED, PLAYER_AXIS_SPEED,
                                          -10, 10);
    currentAngle = qapproachlinear(desiredAngle, currentAngle, frametime * 100);
    SetAngle(currentAngle);
}

void Player::UpdateShoot(float frametime)
{
    if (!Input::GetInstance()->IsButtonDown(BT_SHOOT))
        return;

    if (!CanShoot())
        return;

    Shoot(GetForward(), GetSize().x * 0.45f);
}

float Player::GetOpacity() const
{
    if (IsImmunityActive())
    {
        float fraction = 1.0f - (immunityTimer - GetGameContext()->GetGameTime()) / IMMUNITY_DURATION;
        fraction = qAbs(sin(fraction * C_PI * 6.5f));
        return fraction;
    }

    return BaseClass::GetOpacity();
}

bool Player::IsImmunityActive() const
{
    return immunityTimer > GetGameContext()->GetGameTime();
}

bool Player::IsSpawning() const
{
    return spawnTimer > GetGameContext()->GetGameTime();
}

void Player::CreateParticles()
{
    if (!idleParticle.isNull())
        idleParticle->StopEmission();

    idleParticle = GetGameContext()->GetParticleRoot()->CreateParticles("player_0_idle");

    if (!idleParticle.isNull())
        idleParticle->SetParticleParent(this);
}

void Player::ResetSpawnAnimation()
{
    spawnFinished = false;
    spawnTimer = GetGameContext()->GetGameTime() + SPAWN_DURATION;
    immunityTimer = GetGameContext()->GetGameTime() + IMMUNITY_DURATION;

    SetAngle(0);
}

bool Player::IsAlive() const
{
    if (IsSpawning())
        return false;

    return BaseClass::IsAlive();
}

void Player::TakeDamage(const Damage_t &damage)
{
    if (shieldTime <= 0.0f)
    {
        BaseClass::TakeDamage(damage);
    }
}

void Player::OnDamage(const Damage_t &damage)
{
    BaseClass::OnDamage(damage);

    shieldTime = SHIELD_FADE_DURATION + C_PI_F * 5.0f;

    if (!shieldParticle.isNull())
        shieldParticle->StopEmission();

    shieldParticle = GetGameContext()->GetParticleRoot()->CreateParticles("player_shield_damage");

    if (!shieldParticle.isNull())
        shieldParticle->SetParticleParent(this);

    AudioManager::GetInstance()->PlaySoundSample("misc/shield_hit.wav");

    if (GetHealth() == 1)
    {
        AudioManager::GetInstance()->PlaySoundSample("misc/shield_alarm.wav", 0.8f);

        shieldDownTime = SHIELD_DOWN_DELAY + SHIELD_DOWN_DURATION;
    }
}

void Player::OnKilled(const Damage_t *damage)
{
    respawnDelay = GetGameContext()->GetGameTime() + RESPAWN_DELAY;

    if (!idleParticle.isNull())
        idleParticle->StopEmission();

    GetGameContext()->GetParticleRoot()->CreateParticles("explosion_player_0", GetOrigin(), -GetForward());

    AudioManager::GetInstance()->PlaySoundSample("blast/blast_player_0.wav", 0.7f);

    SetVelocity(vec2_origin);

    GetGameContext()->PlayerDied();
}

void Player::OnRender(const render_context_t &context)
{
    BaseClass::OnRender(context);

    const bool bShieldVisible = shieldTime > 0.0f;
    const bool bShieldDownVisible = shieldDownTime > 0.0f;

    if (IsVisible()
            && (bShieldVisible  || bShieldDownVisible))
    {
        Vector2D originAdjusted = GetOrigin();
        Camera::GetInstance()->ToScreen(originAdjusted);

        const float oldOpacity = context.painter->opacity();

        if (bShieldVisible)
        {
            float shieldOpacity;
            if (shieldTime >SHIELD_FADE_DURATION)
            {
                float remappedTime = shieldTime - SHIELD_FADE_DURATION - C_PI_F * 0.5f;
                shieldOpacity = sin(remappedTime) * 0.5f + 0.5f;
            }
            else
            {
                shieldOpacity = shieldTime / SHIELD_FADE_DURATION;
                shieldOpacity *= shieldOpacity;
            }

            Vector2D sizeScaled = GetSize() * 1.1f;
            Camera::GetInstance()->Scale(sizeScaled);

            context.painter->setOpacity(oldOpacity * shieldOpacity);

            shieldMaterial->Render(originAdjusted, GetAngle(), sizeScaled, context);
        }

        if (bShieldDownVisible
                && shieldDownTime <= SHIELD_DOWN_DURATION)
        {
            float shieldDownOpacity = sin(shieldDownTime - C_PI_F * 0.5f) * 0.5f + 0.5f;

            Vector2D sizeScaled = GetSize() * 1.05f;
            Camera::GetInstance()->Scale(sizeScaled);

            context.painter->setOpacity(oldOpacity * shieldDownOpacity);

            shieldDownMaterial->Render(originAdjusted, GetAngle(), sizeScaled, context);
        }

        context.painter->setOpacity(oldOpacity);
    }
}

void Player::SetPlayerSprite(const char *spriteName)
{
    QString shieldMaterialName(spriteName);
    shieldMaterialName += "_shield";

    QString shieldDownMaterialName(spriteName);
    shieldDownMaterialName += "_shield_down";

    SetMaterial(spriteName);

    shieldMaterial = MaterialPrecache::GetInstance()->GetMaterial(shieldMaterialName.toStdString().c_str());
    shieldDownMaterial = MaterialPrecache::GetInstance()->GetMaterial(shieldDownMaterialName.toStdString().c_str());
}
