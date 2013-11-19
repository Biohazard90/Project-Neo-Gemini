#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

#include "vector2d.h"

class ICollidable
{
public:
    virtual ~ICollidable(){}

    virtual bool IsSolid() const = 0;
    virtual bool ShouldCollide(ICollidable *other) const = 0;
    virtual const Vector2D &GetCollisionOrigin() const = 0;
    virtual void GetCollisionBounds(Vector2D &mins, Vector2D &maxs) const = 0;

    virtual void OnCollision(ICollidable *other) = 0;
};

#endif // ICOLLIDABLE_H
