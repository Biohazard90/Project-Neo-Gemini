#ifndef PLAYER_H
#define PLAYER_H

#include "ship.h"

#include <QPointer>

class ParticleSystem;

class Player : public Ship
{
    DECLARE_CLASS(Player, Ship);
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

    virtual bool IsAlive() const;
    virtual void OnKilled(const Damage_t *damage);

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

    QPointer<ParticleSystem> idleParticle;

    float immunityTimer;
    float spawnTimer;
    float respawnDelay;

    bool spawnFinished;

};

#endif // PLAYER_H
