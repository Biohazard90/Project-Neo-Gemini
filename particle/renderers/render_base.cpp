
#include "render_base.h"

#include <QPainter>
#include <QtXml>


ParticleRenderBase::ParticleRenderBase()
{
    screenspace_position = false;
    screenspace_scale = false;
    unscaled = false;
}

ParticleRenderBase::~ParticleRenderBase()
{
}

bool ParticleRenderBase::IsScreenspacePosition()
{
    return screenspace_position;
}

bool ParticleRenderBase::IsScreenspaceScale()
{
    return screenspace_scale;
}

bool ParticleRenderBase::IsUnscaled()
{
    return unscaled;
}

bool ParticleRenderBase::ParseFromXML(const char *tag, const char *data, QDomElement *source)
{
    PARTICLE_PARAMS_BEGIN
        PARTICLE_PARAM_BOOL(screenspace_position);
        PARTICLE_PARAM_BOOL(screenspace_scale);
        PARTICLE_PARAM_BOOL(unscaled);
    PARTICLE_PARAMS_END;
}
