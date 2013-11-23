#ifndef MENUBACKGROUND_H
#define MENUBACKGROUND_H

#include "../isimulated.h"
#include "rendercontext.h"
#include <QGLWidget>

class MenuBackground : public QObject, ISimulated
{
    Q_OBJECT
public:
    explicit MenuBackground(QWidget *parent = 0);
    ~MenuBackground();

    virtual void paintEvent(render_context_t &context);

    virtual void OnSimulate(float frametime);
    
signals:
    
public slots:

private:
    float m_flPanels[10];
    
};


#endif // MENUBACKGROUND_H
