#ifndef PASSTHROUGHPAINTER_H
#define PASSTHROUGHPAINTER_H

#include "ipainter.h"

class PassthroughPainter : public IPainter
{
    QPainter &inner;

public:
    PassthroughPainter(QPainter &target);

    virtual void setCompositionMode(QPainter::CompositionMode mode)
    {
        inner.setCompositionMode(mode);
    }

    virtual void setFont(const QFont &f)
    {
        inner.setFont(f);
    }

    virtual void setPen(const QColor &color)
    {
        inner.setPen(color);
    }

    virtual void setPen(const QPen &pen)
    {
        inner.setPen(pen);
    }

    virtual void setPen(Qt::PenStyle style)
    {
        inner.setPen(style);
    }

    virtual void setBrush(const QBrush &brush)
    {
        inner.setBrush(brush);
    }

    virtual void setBrush(Qt::BrushStyle style)
    {
        inner.setBrush(style);
    }

    virtual qreal opacity() const
    {
        return inner.opacity();
    }
    virtual void setOpacity(qreal opacity)
    {
        inner.setOpacity(opacity);
    }

    virtual void save()
    {
        inner.save();
    }

    virtual void restore()
    {
        inner.restore();
    }

    virtual void scale(qreal sx, qreal sy)
    {
        inner.scale(sx, sy);
    }

    virtual void shear(qreal sh, qreal sv)
    {
        inner.shear(sh, sv);
    }

    virtual void rotate(qreal a)
    {
        inner.rotate(a);
    }

    virtual void translate(const QPointF &offset)
    {
        inner.translate(offset);
    }

    virtual void translate(const QPoint &offset)
    {
        inner.translate(offset);
    }

    virtual void translate(qreal dx, qreal dy)
    {
        inner.translate(dx, dy);
    }

    virtual void drawLine(const QLineF &line)
    {
        inner.drawLine(line);
    }

    virtual void drawLine(const QLine &line)
    {
        inner.drawLine(line);
    }

    virtual void drawLine(int x1, int y1, int x2, int y2)
    {
        inner.drawLine(x1, y1, x2, y2);
    }

    virtual void drawLine(const QPoint &p1, const QPoint &p2)
    {
        inner.drawLine(p1, p2);
    }

    virtual void drawLine(const QPointF &p1, const QPointF &p2)
    {
        inner.drawLine(p1, p2);
    }

    virtual void drawRect(const QRectF &rect)
    {
        inner.drawRect(rect);
    }

    virtual void drawRect(int x1, int y1, int w, int h)
    {
        inner.drawRect(x1, y1, w, h);
    }

    virtual void drawRect(const QRect &rect)
    {
        inner.drawRect(rect);
    }

    virtual void drawEllipse(const QRectF &r)
    {
        inner.drawEllipse(r);
    }

    virtual void drawEllipse(const QRect &r)
    {
        inner.drawEllipse(r);
    }

    virtual void drawEllipse(int x, int y, int w, int h)
    {
        inner.drawEllipse(x, y, w, h);
    }

    virtual void drawEllipse(const QPointF &center, qreal rx, qreal ry)
    {
        inner.drawEllipse(center, rx, ry);
    }

    virtual void drawEllipse(const QPoint &center, int rx, int ry)
    {
        inner.drawEllipse(center, rx, ry);
    }

    virtual void drawImage(int x, int y, const QImage &image)
    {
        inner.drawImage(x, y, image);
    }

    virtual void drawText(int x, int y, const QString &s)
    {
        inner.drawText(x, y, s);
    }

    virtual void setRenderHint(QPainter::RenderHint hint, bool on)
    {
        inner.setRenderHint(hint, on);
    }

    virtual void setRenderHints(QPainter::RenderHints hints, bool on)
    {
        inner.setRenderHints(hints, on);
    }
};

#endif // PASSTHROUGHPAINTER_H
