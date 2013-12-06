#ifndef DESTRUCTIBLE_H
#define DESTRUCTIBLE_H

#include "sprite.h"
#include "gameutil.h"

class Destructible : public Sprite
{
    DECLARE_CLASS(Destructible, Sprite);

public:
    Destructible();

    virtual void Init(const Resource_Destructible_t &data);

protected:
    virtual void OnDamage(const Damage_t &damage);
    virtual void OnKilled(const Damage_t *damage);

    void SendDestroyedEvent();

private:
    void EmitDamageParticles(const char *particleName, const Damage_t *damage, Vector2D position);

    Resource_Destructible_t data;
};

#endif // DESTRUCTIBLE_H
