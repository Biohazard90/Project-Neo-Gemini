#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include "iparticlerenderer.h"


class ParticleRenderBase : public IParticleRenderer
{
public:
    ParticleRenderBase();
    virtual ~ParticleRenderBase();

    virtual void Begin(const render_context_t &context) {}
    virtual void End(const render_context_t &context) {}

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source);

protected:

    bool IsScreenspacePosition();
    bool IsScreenspaceScale();
    bool IsUnscaled();

private:
    bool screenspace_position;
    bool screenspace_scale;
    bool unscaled;
};


#endif // RENDER_BASE_H
