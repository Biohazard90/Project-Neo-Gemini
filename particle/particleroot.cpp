#include "base.h"
#include "particlebase.h"

#include <QPainter>
#include <QThread>


ParticleRoot::ParticleRoot()
{
}

ParticleRoot::~ParticleRoot()
{
    DESTROY_QVECTOR(systems);
}

void ParticleRoot::OnParentRemoved(IParticleParent *potentialParent)
{
    FOREACH_QVECTOR_FAST(systems, ParticleSystem*, s)
    {
        s->OnParentRemoved(potentialParent);
    }
    FOREACH_QVECTOR_FAST_END;
}

void ParticleRoot::OnSimulate(float frametime)
{
    FOREACH_QVECTOR(systems, ParticleSystem*, p)
    {
        p->OnSimulate(frametime);

        if (p->ShouldDie())
        {
            systems.remove(ip);
            ip--;
            delete p;
        }
    }
    FOREACH_QVECTOR_END;
}

void ParticleRoot::OnRender(const render_context_t &context)
{
    context.painter->save();

    FOREACH_QVECTOR_FAST(systems, ParticleSystem*, p)
    {
        p->OnRender(context);
    }
    FOREACH_QVECTOR_FAST_END;

    context.painter->restore();
}

ParticleSystem *ParticleRoot::CreateParticles(const char *name, Vector2D origin, Vector2D forward)
{
    ParticleSystem *system = ParticlePrecache::GetInstance()->CreateParticleSystem(name);
    if (system == NULL)
    {
        DBGWARNING("!! Failed to create particle system:" << name << "!");
        return NULL;
    }

    DBGMSG("Created particle system:" << name);
    forward.Normalize();

    system->SetOrigin(origin);
    system->SetForward(forward);

    systems.push_back(system);
    return system;
}
