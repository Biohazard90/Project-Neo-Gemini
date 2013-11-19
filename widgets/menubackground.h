#ifndef MENUBACKGROUND_H
#define MENUBACKGROUND_H

#include "../isimulated.h"
#include <QWidget>

class MenuBackground : public QWidget, ISimulated
{
    Q_OBJECT
public:
    explicit MenuBackground(QWidget *parent = 0);
    ~MenuBackground();

    virtual void paintEvent(QPaintEvent *);

    virtual void OnSimulate(float frametime);
    
signals:
    
public slots:

private:
    float m_flPanels[10];
    
};


#endif // MENUBACKGROUND_H
