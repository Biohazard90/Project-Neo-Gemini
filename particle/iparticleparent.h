#ifndef IPARTICLEPARENT_H
#define IPARTICLEPARENT_H

#include "vector2d.h"

class IParticleParent
{
public:
    virtual ~IParticleParent(){}

    virtual const Vector2D &GetParticleOrigin() const = 0;
    virtual Vector2D GetParticleForward() const = 0;
    virtual const float &GetParticleAngle() const = 0;
    virtual float GetParticleOpacity() const = 0;
};

#endif // IPARTICLEPARENT_H
