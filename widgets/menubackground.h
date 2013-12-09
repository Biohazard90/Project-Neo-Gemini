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

    void DrawGLRoundedRect(const QRectF &rect, float roundPerc);
    void DrawGLRoundedCorner(const Vector2D &center,float startDegrees, float endDegrees,
                             float size, int subDiv);

    QColor backgroundInner;
    QColor backgroundOuter;
    
};


#endif // MENUBACKGROUND_H
