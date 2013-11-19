#ifndef BOSSSHIP_H
#define BOSSSHIP_H

#include "enemyship.h"

class BossShip : public EnemyShip
{
    DECLARE_CLASS(BossShip, EnemyShip);

public:
    BossShip();

    virtual void Spawn();

    virtual bool IsSolid() const;
    virtual void OnCollision(ICollidable *);

    virtual void OnSimulate(float frametime);

    virtual void OnKilled(const Damage_t *damage);

    virtual bool AutoRemoveOffscreen() const { return false; }

private:

    float deathFXTimer;
    float removalTimer;
};

#endif // BOSSSHIP_H
