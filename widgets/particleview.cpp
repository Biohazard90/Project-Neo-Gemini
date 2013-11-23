#include "particleview.h"

#include "particlebase.h"
#include "passthroughpainter.h"


ParticleView::ParticleView(QWidget *parent) :
    QObject(parent)
{
    particleRoot = new ParticleRoot();

    SimulationList::GetInstance()->AddSimulationObject(this);
}

ParticleView::~ParticleView()
{
    SimulationList::GetInstance()->RemoveSimulationObject(this);
}

void ParticleView::OnSimulate(float frametime)
{
    particleRoot->OnSimulate(frametime);
}

void ParticleView::paintEvent(render_context_t &context)
{
    particleRoot->OnRender(context);
}

void ParticleView::resizeEvent(QResizeEvent *e)
{
}

ParticleRoot *ParticleView::GetParticleRoot()
{
    return particleRoot;
}
