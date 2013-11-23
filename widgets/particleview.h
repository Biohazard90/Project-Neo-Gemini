#ifndef PARTICLEVIEW_H
#define PARTICLEVIEW_H

#include <QGLWidget>
#include "isimulated.h"
#include "rendercontext.h"

class ParticleRoot;

class ParticleView : public QObject, ISimulated
{
    Q_OBJECT
public:
    explicit ParticleView(QWidget *parent = 0);
    ~ParticleView();

    virtual void OnSimulate(float frametime);
    virtual void paintEvent(render_context_t &context);
    virtual void resizeEvent(QResizeEvent *);

    ParticleRoot *GetParticleRoot();
    
signals:
    
public slots:
    
private:
    ParticleRoot *particleRoot;

};

#endif // PARTICLEVIEW_H
