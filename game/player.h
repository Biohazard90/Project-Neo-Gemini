#ifndef PLAYER_H
#define PLAYER_H

#include "ship.h"

#include <QPointer>

class ParticleSystem;

class Player : public Ship
{
    DECLARE_ENTITY_CLASS(Player, Ship);
public:
    Player();

    virtual void Spawn();

    virtual void OnSimulate(float frametime);
    virtual void OnMove(float frametime);

    virtual bool IsVisible() const;

    virtual bool IsPlayer() const { return true; }

    virtual bool IsSolid() const;
    virtual bool ShouldCollide(ICollidable *) const;

    virtual float GetOpacity() const;
    virtual void OnRender(const render_context_t &context);

    virtual void SetHealth(const int &health);
    virtual bool IsAlive() const;
    virtual void TakeDamage(const Damage_t &damage);
    virtual void OnDamage(const Damage_t &damage);
    virtual void OnKilled(const Damage_t *damage);

    void SetPlayerSprite(const char *spriteName);

private:

    bool IsImmunityActive() const;
    bool IsSpawning() const;

    bool HandleSpawn();
    bool HandleRespawn();

    void UpdateMove(float frametime);
    void UpdateSway(float frametime);
    void UpdateShoot(float frametime);

    void CreateParticles();
    void ResetSpawnAnimation();

    void SendHealthChangedEvent(int health, bool isDecreasing, const Damage_t *damage);

    QPointer<ParticleSystem> idleParticle;
    QPointer<ParticleSystem> shieldParticle;

    float immunityTimer;
    float spawnTimer;
    float respawnDelay;

    bool spawnFinished;

    Material *shieldMaterial;
    Material *shieldDownMaterial;
    float shieldTime;
    float shieldDownTime;

};

#endif // PLAYER_H
