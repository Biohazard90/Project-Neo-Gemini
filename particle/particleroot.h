#ifndef PARTICLEROOT_H
#define PARTICLEROOT_H

#include <QVector>

class ParticleSystem;
class IParticleParent;
struct render_context_t;

class ParticleRoot
{
public:
    ParticleRoot();
    virtual ~ParticleRoot();

    virtual void OnSimulate(float frametime);
    virtual void OnRender(const render_context_t &context);

    ParticleSystem *CreateParticles(const char *name,
                                    Vector2D origin = vec2_origin,
                                    Vector2D forward = vec2_forward);

    void OnParentRemoved(IParticleParent *potentialParent);
    
signals:
    
public slots:

private:
    QVector<ParticleSystem*> systems;
    
};


#endif // PARTICLEROOT_H
