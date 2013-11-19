#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <QVector>
#include <QList>

#include "iparticlecontext.h"
#include "particle.h"
#include "rendercontext.h"
#include "vector2d.h"

class IParticleEmitter;
class IParticleInitializer;
class IParticleOperator;
class IParticleRenderer;
class IParticleParent;
class ParticlePrecache;
class ParticleRoot;
class QPainter;

#define MAX_CONTROLPOINTS 4

class ParticleSystem : public QObject, public IParticleContext
{
    Q_OBJECT
    friend class ParticlePrecache;
    friend class ParticleRoot;

    ParticleSystem();
    ~ParticleSystem();
public:

    virtual const Vector2D &GetOrigin() const
    {
        return origin;
    }

    FORCEINLINE void SetOrigin(const Vector2D &origin)
    {
        this->origin = origin;
    }

    virtual const Vector2D &GetLastOrigin() const
    {
        return lastOrigin;
    }

    virtual const Vector2D &GetForward() const
    {
        return forward;
    }

    virtual const Vector2D &GetControlPoint(const int &index) const
    {
        return controlPointOrigin[index];
    }

    FORCEINLINE void SetForward(const Vector2D &forward)
    {
        this->forward = forward;
    }

    virtual const float &GetFrametime() const
    {
        return frametime;
    }

    virtual void EmitChildParticle();
    virtual void SetChildControlPoint(int controlpoint, int component, float value);

    void StopEmission();
    void StopEmissionAndParticles();
    void OnParentRemoved(IParticleParent *potentialParent);

    bool ShouldDie() const;

    void OnSimulate(float frametime);
    void OnRender(const render_context_t &context);

    FORCEINLINE IParticleParent *GetParticleParent()
    {
        return parent;
    }

    FORCEINLINE void SetParticleParent(IParticleParent *parent)
    {
        this->parent = parent;
    }

private:
    void OnSimulateInternal(float frametime, Vector2D offset);
    void OnParentRemovedInternal(IParticleParent *potentialParent);
    void InitializeParticle(particle_t *newParticle);

    QList<particle_t*> particles;

    QVector<IParticleEmitter*> emitters;
    QVector<IParticleInitializer*> initializers;
    QVector<IParticleOperator*> operators;
    QVector<IParticleRenderer*> renderers;

    QVector<ParticleSystem*> children;

    IParticleParent *parent;

    Vector2D origin;
    Vector2D lastOrigin;
    Vector2D forward;
    float frametime;

    bool initialized;

    Vector2D controlPointOrigin[MAX_CONTROLPOINTS];
};

#endif // PARTICLESYSTEM_H
