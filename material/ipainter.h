#ifndef IPAINTER_H
#define IPAINTER_H

#include <QPainter>

class IPainter
{
public:
    virtual ~IPainter(){}

    virtual void setCompositionMode(QPainter::CompositionMode mode) = 0;

    virtual void setFont(const QFont &f) = 0;

    virtual void setPen(const QColor &color) = 0;
    virtual void setPen(const QPen &pen) = 0;
    virtual void setPen(Qt::PenStyle style) = 0;

    virtual void setBrush(const QBrush &brush) = 0;
    virtual void setBrush(Qt::BrushStyle style) = 0;

    virtual qreal opacity() const = 0;
    virtual void setOpacity(qreal opacity) = 0;

    virtual void save() = 0;
    virtual void restore() = 0;

    virtual void scale(qreal sx, qreal sy) = 0;
    virtual void shear(qreal sh, qreal sv) = 0;
    virtual void rotate(qreal a) = 0;

    virtual void translate(const QPointF &offset) = 0;
    virtual void translate(const QPoint &offset) = 0;
    virtual void translate(qreal dx, qreal dy) = 0;

    virtual void drawLine(const QLineF &line) = 0;
    virtual void drawLine(const QLine &line) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void drawLine(const QPoint &p1, const QPoint &p2) = 0;
    virtual void drawLine(const QPointF &p1, const QPointF &p2) = 0;

    virtual void drawRect(const QRectF &rect) = 0;
    virtual void drawRect(int x1, int y1, int w, int h) = 0;
    virtual void drawRect(const QRect &rect) = 0;

    virtual void drawEllipse(const QRectF &r) = 0;
    virtual void drawEllipse(const QRect &r) = 0;
    virtual void drawEllipse(int x, int y, int w, int h) = 0;

    virtual void drawEllipse(const QPointF &center, qreal rx, qreal ry) = 0;
    virtual void drawEllipse(const QPoint &center, int rx, int ry) = 0;

    virtual void drawImage(int x, int y, const QImage &image) = 0;

    virtual void drawText(int x, int y, const QString &s) = 0;

    virtual void setRenderHint(QPainter::RenderHint hint, bool on = true) = 0;
    virtual void setRenderHints(QPainter::RenderHints hints, bool on = true) = 0;
};

#endif // IPAINTER_H
