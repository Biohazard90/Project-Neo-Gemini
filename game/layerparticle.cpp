#include "layerparticle.h"
#include "particlebase.h"


LayerParticle::LayerParticle()
{
    particleRoot = new ParticleRoot();
}

LayerParticle::~LayerParticle()
{
    delete particleRoot;
}

void LayerParticle::OnSimulate(float frametime)
{
    particleRoot->OnSimulate(frametime);
}

void LayerParticle::OnRender(const render_context_t &context)
{
    particleRoot->OnRender(context);
}

void LayerParticle::AddParticle(const char *name, Vector2D origin)
{
    particleRoot->CreateParticles(name, origin);
}
