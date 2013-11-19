#ifndef PARTICLEVIEW_H
#define PARTICLEVIEW_H

#include <QWidget>
#include "isimulated.h"

class ParticleRoot;

class ParticleView : public QWidget, ISimulated
{
    Q_OBJECT
public:
    explicit ParticleView(QWidget *parent = 0);
    ~ParticleView();

    virtual void OnSimulate(float frametime);
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

    ParticleRoot *GetParticleRoot();
    
signals:
    
public slots:
    
private:
    ParticleRoot *particleRoot;

};

#endif // PARTICLEVIEW_H
