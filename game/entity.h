#ifndef ENTITY_H
#define ENTITY_H

#include "platform_warnings.h"

#include "vector2d.h"

#include "irenderable.h"
#include "isimulated.h"
#include "iparticleparent.h"
#include "icollidable.h"

#include "macros.h"
#include "platform.h"

#include "damage.h"

class IGameContext;

class Entity : public IRenderable,
        public ISimulated,
        public IParticleParent,
        public ICollidable
{
    Entity(const Entity &other);
public:
    Entity();
    virtual ~Entity();

    virtual void Spawn();
    virtual void OnRemove();
    virtual void Remove();

    virtual bool IsPlayer() const { return false; }
    virtual bool IsEnemy() const { return false; }
    virtual bool IsObstacle() const { return false; }
    virtual bool IsProjectile() const { return false; }

    virtual const Vector2D &GetOrigin() const;
    virtual const Vector2D &GetVelocity() const;
    virtual float GetAngle() const;
    virtual float GetAngularVelocity() const;
    virtual Vector2D GetForward() const;
    virtual Vector2D GetSize() const;

    virtual void SetOrigin(const Vector2D &origin);
    virtual void SetVelocity(const Vector2D &velocity);
    virtual void SetAngle(const float &angle);
    virtual void SetAngularVelocity(const float &angularVelocity);
    virtual void SetSize(Vector2D size);

    virtual void Teleport(const Vector2D &origin);

    virtual void OnSimulate(float frametime);
    virtual void OnMove(float frametime);
    virtual void OnRender(const render_context_t &context);

    virtual bool IsVisible() const { return true; }

    virtual const Vector2D &GetParticleOrigin() const;
    virtual Vector2D GetParticleForward() const;
    virtual const float &GetParticleAngle() const;
    virtual float GetParticleOpacity() const { return 1.0f; }

    virtual bool IsSolid() const { return false; }
    virtual bool ShouldCollide(ICollidable *) const { return false; }
    virtual void GetCollisionBounds(Vector2D &mins, Vector2D &maxs) const;
    virtual const Vector2D &GetCollisionOrigin() const { return GetOrigin(); }
    virtual void OnCollision(ICollidable *);

    void SetGameContext(IGameContext *gameContext);

    enum EntityFlags_e
    {
        EF_KILL = (1 << 0),
        EF_TELEPORTED = (1 << 1)
    };

    virtual int GetFlags() const;
    virtual void SetFlags(const int &flags);
    virtual void AddFlags(const int &flags);
    virtual void RemoveFlags(const int &flags);
    virtual bool HasFlags(const int &flags);

    virtual int GetHealth() const;
    virtual void SetHealth(const int &health);
    virtual int GetMaxHealth() const;
    virtual void SetMaxHealth(const int &maxHealth);
    virtual bool IsAlive() const { return GetHealth() > 0; }
    virtual bool IsDead() const { return !IsAlive(); }
    virtual void Kill(const Damage_t *damage = nullptr);
    virtual void TakeDamage(const Damage_t &damage);
    virtual void TakeDamage(const int &damage, Entity *inflictor = nullptr, const Vector2D &direction = Vector2D(1, 0));
    virtual bool IsAutoRemoveEnabled() const { return autoRemove; }
    virtual void SetAutoRemoveEnabled(bool enabled) { autoRemove = enabled; }

    virtual Entity *GetOwner() const { return owner; }
    virtual void SetOwner(Entity *owner) { this->owner = owner; }

protected:
    inline IGameContext *GetGameContext() const { return gameContext; }

    virtual void OnDamage(const Damage_t &damage);
    virtual void OnKilled(const Damage_t *damage = nullptr);

private:
    Vector2D origin;
    Vector2D velocity;
    Vector2D forward;
    Vector2D size;

    float angle;
    float angularVelocity;

    IGameContext *gameContext;

    int flags;
    int health;
    int maxHealth;
    bool autoRemove;

    Entity *owner;
};

#endif // ENTITY_H
