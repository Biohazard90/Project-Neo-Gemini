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

    if (pGlobals->antialiasing)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::TextAntialiasing, true);
        painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LINE_SMOOTH);
    }
    else
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setRenderHint(QPainter::TextAntialiasing, false);
        painter->setRenderHint(QPainter::HighQualityAntialiasing, false);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

        glDisable(GL_MULTISAMPLE);
        glDisable(GL_LINE_SMOOTH);
    }

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
