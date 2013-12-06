#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H

#include "ship.h"
#include "gameutil.h"

#include "iai.h"

class EnemyShip : public Ship
{
    DECLARE_ENTITY_CLASS(EnemyShip, Ship);
public:
    EnemyShip();

    virtual const char *GetEntityResourceClass() { return data.name.c_str(); }

    virtual void Spawn();
    virtual void OnRemove();

    virtual bool IsEnemy() const { return true; }

    virtual bool AutoRemoveOffscreen() const { return true; }

    virtual bool ShouldCollide(ICollidable *) const;
    virtual void OnCollision(ICollidable *);

    virtual void OnSimulate(float frametime);
    virtual void OnMove(float frametime);

    virtual void Init(const Resource_Fighter_t &data, IAI *aiStrategy);

protected:
    IAI *aiStrategy;

    const Resource_Fighter_t &GetFighterData() { return data; }

private:
    Resource_Fighter_t data;

};

#endif // ENEMYSHIP_H
