#include "particleview.h"

#include "particlebase.h"
#include "passthroughpainter.h"


ParticleView::ParticleView(QWidget *parent) :
    QWidget(parent)
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

    update();
}

void ParticleView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    PassthroughPainter passthrough(painter);

    render_context_t c;
    c.painter = &passthrough;
    //c.painter = &painter;
    c.x = x();
    c.y = y();
    c.w = width();
    c.h = height();

    particleRoot->OnRender(c);
}

void ParticleView::resizeEvent(QResizeEvent *e)
{
}

ParticleRoot *ParticleView::GetParticleRoot()
{
    return particleRoot;
}
