
#include "particlebase.h"

#include <QtXml>


class ParticleOpLockToSystemOrigin : public IParticleOperator
{
public:
    ParticleOpLockToSystemOrigin()
    {
    }

    virtual IParticleFunction *Copy()
    {
        return new ParticleOpLockToSystemOrigin(*this);
    }

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source)
    {
        return true;
    }

    virtual void SimulateParticle(IParticleContext *context, particle_t &particle)
    {
        Vector2D moved = context->GetOrigin() - context->GetLastOrigin();
        particle.data_runtime.origin += moved;
    }
};

REGISTER_PARTICLE_FUNCTION(ParticleOpLockToSystemOrigin, oplocktosystemorigin);
