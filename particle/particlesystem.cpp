#include "particlebase.h"

#include <QPainter>

ParticleSystem::ParticleSystem()
{
    frametime = 0;
    initialized = false;
    parent = nullptr;

    forward = Vector2D::AngleDirection(0);
}

ParticleSystem::~ParticleSystem()
{
    DESTROY_QVECTOR(children);

    DESTROY_QVECTOR(emitters);
    DESTROY_QVECTOR(initializers);
    DESTROY_QVECTOR(operators);
    DESTROY_QVECTOR(renderers);

    DESTROY_QLIST(particles);
}

void ParticleSystem::StopEmission()
{
    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        s->StopEmission();
    }
    FOREACH_QVECTOR_FAST_END;

    DESTROY_QVECTOR(emitters);
}

void ParticleSystem::StopEmissionAndParticles()
{
    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        s->StopEmissionAndParticles();
    }
    FOREACH_QVECTOR_FAST_END;

    DESTROY_QVECTOR(emitters);
    DESTROY_QLIST(particles);
}

void ParticleSystem::OnParentRemoved(IParticleParent *potentialParent)
{
    if (parent == potentialParent)
    {
        origin.Rotate(potentialParent->GetParticleAngle());
        origin += potentialParent->GetParticleOrigin();
        forward.Rotate(potentialParent->GetParticleAngle());
    }

    OnParentRemovedInternal(potentialParent);
}

void ParticleSystem::OnParentRemovedInternal(IParticleParent *potentialParent)
{
    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        s->OnParentRemovedInternal(potentialParent);
    }
    FOREACH_QVECTOR_FAST_END;

    if (parent == potentialParent)
    {
        parent = nullptr;
        StopEmission();
    }
}

bool ParticleSystem::ShouldDie() const
{
    FOREACH_QVECTOR_FAST_CONST(children, ParticleSystem*, s)
    {
        if (!s->ShouldDie())
            return false;
    }
    FOREACH_QVECTOR_FAST_END;

    return emitters.size() == 0
            && particles.size() == 0;
}

void ParticleSystem::OnSimulate(float frametime)
{
    OnSimulateInternal(frametime, origin);
}

void ParticleSystem::OnSimulateInternal(float frametime, Vector2D offset)
{
    Vector2D oldForward(forward);

    if (parent != nullptr)
    {
        origin = parent->GetParticleOrigin();
        Vector2D localOffset(offset);
        localOffset.Rotate(parent->GetParticleAngle());
        origin += localOffset;

        forward.Rotate(parent->GetParticleAngle());
    }
    else
    {
        origin = offset;
    }

    if (!initialized)
    {
        lastOrigin = origin;
        initialized = true;
    }

    this->frametime = frametime;

    QList<particle_t*> spawned;

    FOREACH_QVECTOR(emitters, IParticleEmitter*, e)
    {
        if (!e->EmitParticles(this, spawned))
        {
            emitters.remove(ie);
            ie--;
            delete e;
        }
    }
    FOREACH_QVECTOR_END;

    FOREACH_QLIST(spawned, particle_t*, p)
    {
        InitializeParticle(p);
    }
    FOREACH_QLIST_END;

    particles.append(spawned);

    FOREACH_QLIST(particles, particle_t*, p)
    {
        FOREACH_QVECTOR_FAST(operators, IParticleOperator*, op)
        {
            op->SimulateParticle(this, *p);
        }
        FOREACH_QVECTOR_FAST_END

        if (p->ShouldDie())
        {
            particles.removeOne(p);
            delete p;
        }
    }
    FOREACH_QLIST_END;

    FOREACH_QVECTOR(children, ParticleSystem*, s)
    {
        // transform children to space of this system
        Vector2D childOldOrigin(s->origin);
        Vector2D childOldForward(s->forward);

        s->origin.Rotate(forward);
        s->forward.Rotate(forward);

        s->OnSimulateInternal(frametime, s->origin + origin);

        s->origin = childOldOrigin;
        s->forward = childOldForward;

        if (s->ShouldDie())
        {
            children.remove(is);
            is--;
            delete s;
        }
    }
    FOREACH_QVECTOR_END;

    lastOrigin = origin;
    origin = offset;
    forward = oldForward;
}

void ParticleSystem::InitializeParticle(particle_t *newParticle)
{
    newParticle->seed = qfrand() * USHRT_MAX;

    FOREACH_QVECTOR_FAST(initializers, IParticleInitializer*, init)
    {
        init->InitializeParticle(this, *newParticle);
    }
    FOREACH_QVECTOR_FAST_END

    newParticle->data_runtime = newParticle->data_initial;
}

void ParticleSystem::OnRender(const render_context_t &context)
{
    float oldOpacity = context.painter->opacity();

    if (parent != nullptr)
    {
        context.painter->setOpacity(oldOpacity * parent->GetParticleOpacity());
    }

    if (!particles.empty())
    {
        FOREACH_QVECTOR_FAST(renderers, IParticleRenderer*, re)
        {
            re->Begin(context);

            FOREACH_QLIST(particles, particle_t*, p)
            {
                //context.painter->save();

                re->DrawParticle(context, *p);

                //context.painter->restore();
            }
            FOREACH_QLIST_END;

            re->End(context);
        }
        FOREACH_QVECTOR_FAST_END;
    }

    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        s->OnRender(context);
    }
    FOREACH_QVECTOR_FAST_END;

    context.painter->setOpacity(oldOpacity);
}

void ParticleSystem::EmitChildParticle()
{
    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        particle_t *p = new particle_t();
        s->InitializeParticle(p);
        s->particles.push_back(p);
    }
    FOREACH_QVECTOR_FAST_END;
}

void ParticleSystem::SetChildControlPoint(int controlpoint,
                                          int component, float value)
{
    Q_ASSERT(component >= 0 && component <= 1);

    FOREACH_QVECTOR_FAST(children, ParticleSystem*, s)
    {
        s->controlPointOrigin[controlpoint][component] = value;
    }
    FOREACH_QVECTOR_FAST_END;
}
