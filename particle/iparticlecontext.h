#ifndef IPARTICLECONTEXT_H
#define IPARTICLECONTEXT_H

#include "vector2d.h"

class IParticleContext
{
public:
    virtual ~IParticleContext(){}

    virtual const Vector2D &GetOrigin() const = 0;
    virtual const Vector2D &GetLastOrigin() const = 0;
    virtual const Vector2D &GetForward() const = 0;
    virtual const float &GetFrametime() const = 0;

    virtual const Vector2D &GetControlPoint(const int &index) const = 0;

    virtual void EmitChildParticle() = 0;
    virtual void SetChildControlPoint(int controlpoint, int component, float value) = 0;
};

#endif // IPARTICLECONTEXT_H
