#include "base.h"
#include "gameview.h"
#include "rootview.h"

#include <QGLWidget>
#include <QtOpenGL>

GameView::GameView(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void GameView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget)
{
    render_context_t c;

    c.painter = painter;

    painter->setRenderHint(QPainter::Antialiasing, pGlobals->antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing, pGlobals->antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, pGlobals->antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, pGlobals->antialiasing);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    c.x = 0;
    c.y = 0;
    c.w = width();
    c.h = height();

    QPoint mousePos = widget->mapFromGlobal(QCursor::pos());

    c.mx = mousePos.x();
    c.my = mousePos.y();

    extern RootView *VIEW;
    VIEW->externalPaintEvent(c);
    update();
}
