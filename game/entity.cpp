#include "entity.h"

#include "gamebase.h"

Entity::Entity()
    : gameContext(NULL)
    , angle(0)
    , angularVelocity(0)
    , flags(0)
    , health(0)
    , maxHealth(1)
    , owner(NULL)
    , autoRemove(true)
{
    forward = Vector2D::AngleDirection(0);
}

Entity::~Entity()
{
}

void Entity::Spawn()
{
    AddFlags(EF_TELEPORTED);

    SetHealth(GetMaxHealth());
}

void Entity::OnRemove()
{
}

void Entity::Remove()
{
    AddFlags(EF_KILL);
}

const Vector2D &Entity::GetOrigin() const
{
    return origin;
}

const Vector2D &Entity::GetVelocity() const
{
    return velocity;
}

float Entity::GetAngle() const
{
    return angle;
}

float Entity::GetAngularVelocity() const
{
    return angularVelocity;
}

Vector2D Entity::GetForward() const
{
    return Vector2D::AngleDirection(angle);
}

Vector2D Entity::GetSize() const
{
    return size;
}

void Entity::SetOrigin(const Vector2D &origin)
{
    this->origin = origin;
}

void Entity::SetVelocity(const Vector2D &velocity)
{
    this->velocity = velocity;
}

void Entity::SetAngle(const float &angle)
{
    this->angle = angle;
    forward = Vector2D::AngleDirection(angle);
}

void Entity::SetAngularVelocity(const float &angularVelocity)
{
    this->angularVelocity = angularVelocity;
}

void Entity::SetSize(Vector2D size)
{
    this->size = size;
}

void Entity::Teleport(const Vector2D &origin)
{
    SetOrigin(origin);

    AddFlags(EF_TELEPORTED);
}

void Entity::OnCollision(ICollidable *other)
{
}

void Entity::OnSimulate(float frametime)
{
}

void Entity::OnMove(float frametime)
{
    const Vector2D velocity = GetVelocity();

    if (velocity.LengthSqr() > 0)
    {
        origin += velocity * frametime;
    }

    const float angularVelocity = GetAngularVelocity();

    if (angularVelocity != 0.0f)
    {
        angle += angularVelocity * frametime;
        SetAngle( clamp_angle_positive(angle) );
    }
}

void Entity::OnRender(const render_context_t &context)
{
}

const Vector2D &Entity::GetParticleOrigin() const
{
    return GetOrigin();
}

Vector2D Entity::GetParticleForward() const
{
    return GetForward();
}

const float &Entity::GetParticleAngle() const
{
    return angle;
}

void Entity::GetCollisionBounds(Vector2D &mins, Vector2D &maxs) const
{
    maxs = GetSize() * 0.5f;
    mins = -maxs;
}

void Entity::SetGameContext(IGameContext *context)
{
    this->gameContext = context;
}

int Entity::GetFlags() const
{
    return flags;
}

void Entity::SetFlags(const int &flags)
{
    this->flags = flags;
}

void Entity::AddFlags(const int &flags)
{
    this->flags |= flags;
}

void Entity::RemoveFlags(const int &flags)
{
    this->flags &= ~flags;
}

bool Entity::HasFlags(const int &flags)
{
    return (this->flags & flags) == flags;
}

int Entity::GetHealth() const
{
    return health;
}

void Entity::SetHealth(const int &health)
{
    this->health = health;
}

int Entity::GetMaxHealth() const
{
    return maxHealth;
}

void Entity::SetMaxHealth(const int &maxHealth)
{
    this->maxHealth = maxHealth;
}

void Entity::Kill(const Damage_t *damage)
{
    OnKilled(damage);

    if (IsAutoRemoveEnabled())
    {
        Remove();
    }
}

void Entity::TakeDamage(const Damage_t &damage)
{
    int oldHealth = GetHealth();
    int newHealth = oldHealth - damage.damage;
    newHealth = MAX(0, newHealth);
    SetHealth(newHealth);

    if (newHealth <= 0)
    {
        Kill(&damage);
    }
    else
    {
        OnDamage(damage);
    }
}

void Entity::TakeDamage(const int &damage, Entity *inflictor, const Vector2D &direction)
{
    Damage_t d;
    d.damage = damage;
    d.direction = direction;
    d.inflictor = inflictor;
    TakeDamage(d);
}

void Entity::OnDamage(const Damage_t &damage)
{
}

void Entity::OnKilled(const Damage_t *damage)
{
}
