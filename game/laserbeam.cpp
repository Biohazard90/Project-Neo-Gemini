#include "gamebase.h"

#include "qmath.h"

REGISTER_ENTITY_CLASS(LaserBeam, laserbeam);

//TODO: scriptable color and parameters for laser beam

#define LASER_FLASH_DURATION 0.1f;
#define LASER_MIN_SIZE_Y 3.0f
#define LASER_MAX_SIZE_Y 10.0f

LaserBeam::LaserBeam()
{
    timer = 0.0f;
    lifetime = 0.0f;
    spawned = false;

    flashSize = 0.0f;
    flashTimer = 0.0f;
    fixedSize = LASER_MAX_SIZE_Y;
}

void LaserBeam::Init(Entity *owner, const Vector2D &offset, const float &lifetime)
{
    Q_ASSERT(owner != nullptr);

    SetOwner(owner);
    this->offset = offset;
    this->lifetime = lifetime;
}

void LaserBeam::Spawn()
{
    BaseClass::Spawn();

    timer = GetGameContext()->GetGameTime() + lifetime;

    spawned = false;
}

bool LaserBeam::ShouldCollide(ICollidable *object) const
{
    if (!spawned)
    {
        return false;
    }

    Entity *entity = (Entity*)object;

    return entity->IsPlayer()
            || entity->IsObstacle();
}

void LaserBeam::OnCollision(ICollidable *other)
{
    Entity *entity = (Entity*)other;

    Damage_t damage;
    damage.damage = entity->GetHealth();

    if (entity->IsPlayer())
    {
        damage.damage = 1;
    }

    damage.direction = Vector2D(-1, 0);
    damage.inflictor = GetOwner();

    if (GetOwner() != nullptr)
    {
        damage.statsInflictorName = GetOwner()->GetEntityClassName();
        damage.statsInflictorClass = GetOwner()->GetEntityResourceClass();
    }

    damage.statsWeaponName = GetEntityClassName();
    damage.statsWeaponClass = GetEntityResourceClass();

    entity->TakeDamage(damage);

    Vector2D laserCenter = GetOrigin();
    laserCenter.x = entity->GetOrigin().x;

    GetGameContext()->GetParticleRoot()->CreateParticles("laser_cannon_hit", laserCenter, Vector2D(-1, 0));
}

void LaserBeam::OnSimulate(float frametime)
{
    BaseClass::OnSimulate(frametime);

    float laserLength = Camera::GetInstance()->GetWorldMaxs().x - Camera::GetInstance()->GetWorldMins().x;
    float gameTime = GetGameContext()->GetGameTime();

    SetSize(Vector2D(laserLength, LASER_MAX_SIZE_Y));

    Entity *owner = GetOwner();

    if (owner == nullptr
            || timer < gameTime)
    {
        Remove();
    }
    else
    {
        Vector2D center = owner->GetOrigin();
        center.x -= laserLength * 0.5f;
        center += offset;

        if (!spawned)
        {
            Teleport(center);
            spawned = true;
        }
        else
        {
            SetOrigin(center);
        }
    }

    if (flashTimer < gameTime)
    {
        flashSize = fixedSize;
        fixedSize = qlerp(qfrand(), LASER_MIN_SIZE_Y, LASER_MAX_SIZE_Y);
        flashTimer = gameTime + LASER_FLASH_DURATION;
    }
}

void LaserBeam::OnRender(const render_context_t &context)
{
    context.painter->save();

    float lifeFraction = 0.0f;
    if (lifetime > 0.0f)
    {
        lifeFraction = MAX(0, timer - GetGameContext()->GetGameTime()) / lifetime;
    }

    float flashFraction = 1.0f;
    if (flashTimer > 0.0f)
    {
        flashFraction = MAX(0, flashTimer - GetGameContext()->GetGameTime()) / LASER_FLASH_DURATION;
    }

    float baseAlpha = qPow(lifeFraction, 0.5f);
    float flashAlpha = qPow(flashFraction, 2.0f);

    QColor color(160, 255, 175);

    context.painter->setPen(Qt::transparent);
    context.painter->setCompositionMode(QPainter::CompositionMode_Plus);



    Vector2D size = GetSize();
    size.y = flashSize * baseAlpha * 3.0f;
    Vector2D origin = GetOrigin() - size * 0.5f;

    Camera::GetInstance()->ToScreen(origin);
    Camera::GetInstance()->Scale(size);

    color.setAlpha(flashAlpha * baseAlpha * 255);

    context.painter->setBrush(color);
    context.painter->drawRect(origin.x, origin.y, size.x, size.y);

    size = GetSize();
    size.y = fixedSize * baseAlpha;
    origin = GetOrigin() - size * 0.5f;

    Camera::GetInstance()->ToScreen(origin);
    Camera::GetInstance()->Scale(size);

    color.setAlpha(baseAlpha * 255);

    context.painter->setBrush(color);
    context.painter->drawRect(origin.x, origin.y, size.x, size.y);
    context.painter->restore();
}
