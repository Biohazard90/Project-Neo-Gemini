#ifndef DAMAGE_H
#define DAMAGE_H

#include "vector2d.h"

class Entity;

struct Damage_t
{
    Damage_t()
    {
        damage = 0;
        direction.Init(1, 0);
        inflictor = nullptr;
    }

    int damage;

    Vector2D direction;

    Entity *inflictor;

    QString statsInflictorName;
    QString statsInflictorClass;
    QString statsWeaponName;
    QString statsWeaponClass;
};

#endif // DAMAGE_H
