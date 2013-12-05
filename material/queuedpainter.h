#ifndef QUEUEDPAINTER_H
#define QUEUEDPAINTER_H

#include "ipainter.h"
#include "bytebuffer.h"

#include <QStack>

#define SHOW_PAINTER_DEBUG 0

#if SHOW_PAINTER_DEBUG == 1
#include <QDebug>
#define DBGMSG_QUEUE(x) \
    qDebug() << x
#else
#define DBGMSG_QUEUE(x) ((void)0)
#endif

class QueuedPainter : public IPainter
{
    ByteBuffer buffer;

    QStack<qreal> m_opacity;
    const bool cleanup;

    void Init();

public:
    QueuedPainter( char *memory = nullptr, int size = 0, bool autoCleanup = true );
    QueuedPainter( ByteBuffer *externalBuffer, bool autoCleanup = true );
    ~QueuedPainter();

    FORCEINLINE void ExecuteOnDevice(IPainter &painter);
    FORCEINLINE void Flush();

    FORCEINLINE bool CanExecute()
    {
        return buffer.GetReadPosition() == 0
                && buffer.GetWritePosition() > 0;
    }

    FORCEINLINE void Swap(QueuedPainter &other)
    {
        buffer.Swap(other.buffer);

        m_opacity = other.m_opacity;
    }

    FORCEINLINE void Attach(char *memory, int size)
    {
        Q_ASSERT_X(buffer.GetReadPosition() == buffer.GetWritePosition(),
                   "QueuedPainter", "Possible memory leak");

        buffer.Attach(memory, size);
    }

    FORCEINLINE void Detach()
    {
        buffer.Detach();
    }

    FORCEINLINE ByteBuffer *GetBuffer()
    {
        return &buffer;
    }

    enum QUEUE_FUNCTIONS
    {
        F_END = 0,
        F_setCompositionMode,
        F_setFont,
        F_setPen_0,
        F_setPen_1,
        F_setPen_2,
        F_setBrush_0,
        F_setBrush_1,
        F_setOpacity,
        F_save,
        F_restore,
        F_scale,
        F_shear,
        F_rotate,
        F_translate_0,
        F_translate_1,
        F_translate_2,
        F_drawLine_0,
        F_drawLine_1,
        F_drawLine_2,
        F_drawLine_3,
        F_drawLine_4,
        F_drawRect_0,
        F_drawRect_1,
        F_drawRect_2,
        F_drawEllipse_0,
        F_drawEllipse_1,
        F_drawEllipse_2,
        F_drawEllipse_3,
        F_drawEllipse_4,
        F_drawImage,
        F_drawText,
        F_setRenderHint,
        F_setRenderHints,
        F_COUNT
    };

    FORCEINLINE void Finish()
    {
        buffer.WriteUnsignedChar(F_END);
    }

    virtual void setCompositionMode(QPainter::CompositionMode mode)
    {
        buffer.WriteUnsignedChar(F_setCompositionMode);
        buffer.WriteUnsignedChar(mode);
    }

    virtual void setFont(const QFont &f)
    {
        buffer.WriteUnsignedChar(F_setFont);
        buffer.WritePtr(new QFont(f));
    }

    virtual void setPen(const QColor &color)
    {
        buffer.WriteUnsignedChar(F_setPen_0);
        buffer.Write(&color, sizeof(QColor));
    }

    virtual void setPen(const QPen &pen)
    {
        buffer.WriteUnsignedChar(F_setPen_1);
        buffer.WritePtr(new QPen(pen));
    }

    virtual void setPen(Qt::PenStyle style)
    {    
        buffer.WriteUnsignedChar(F_setPen_2);
        buffer.WriteUnsignedChar(style);
    }

    virtual void setBrush(const QBrush &brush)
    {
        buffer.WriteUnsignedChar(F_setBrush_0);
        buffer.WritePtr(new QBrush(brush));
    }

    virtual void setBrush(Qt::BrushStyle style)
    {
        buffer.WriteUnsignedChar(F_setBrush_1);
        buffer.WriteUnsignedChar(style);
    }

    virtual qreal opacity() const
    {
        return m_opacity.back();
    }

    virtual void setOpacity(qreal opacity)
    {
        m_opacity.back() = opacity;
        buffer.WriteUnsignedChar(F_setOpacity);
        buffer.WriteQReal(opacity);
    }


    virtual void save()
    {
        buffer.WriteUnsignedChar(F_save);
        m_opacity.push(m_opacity.back());
    }

    virtual void restore()
    {
        buffer.WriteUnsignedChar(F_restore);
        m_opacity.pop();
    }


    virtual void scale(qreal sx, qreal sy)
    {
        buffer.WriteUnsignedChar(F_scale);
        buffer.WriteQReal(sx);
        buffer.WriteQReal(sy);
    }

    virtual void shear(qreal sh, qreal sv)
    {
        buffer.WriteUnsignedChar(F_shear);
        buffer.WriteQReal(sh);
        buffer.WriteQReal(sv);

    }

    virtual void rotate(qreal a)
    {
        buffer.WriteUnsignedChar(F_rotate);
        buffer.WriteQReal(a);
    }


    virtual void translate(const QPointF &offset)
    {
        buffer.WriteUnsignedChar(F_translate_0);
        buffer.Write(&offset, sizeof(QPointF));
    }

    virtual void translate(const QPoint &offset)
    {
        buffer.WriteUnsignedChar(F_translate_1);
        buffer.Write(&offset, sizeof(QPoint));
    }

    virtual void translate(qreal dx, qreal dy)
    {
        buffer.WriteUnsignedChar(F_translate_2);
        buffer.WriteQReal(dx);
        buffer.WriteQReal(dy);
    }


    virtual void drawLine(const QLineF &line)
    {
        buffer.WriteUnsignedChar(F_drawLine_0);
        buffer.Write(&line, sizeof(QLineF));
    }

    virtual void drawLine(const QLine &line)
    {
        buffer.WriteUnsignedChar(F_drawLine_1);
        buffer.Write(&line, sizeof(QLine));
    }

    virtual void drawLine(int x1, int y1, int x2, int y2)
    {
        buffer.WriteUnsignedChar(F_drawLine_2);
        buffer.WriteInt(x1);
        buffer.WriteInt(y1);
        buffer.WriteInt(x2);
        buffer.WriteInt(y2);
    }

    virtual void drawLine(const QPoint &p1, const QPoint &p2)
    {
        buffer.WriteUnsignedChar(F_drawLine_3);
        buffer.Write(&p1, sizeof(QPoint));
        buffer.Write(&p2, sizeof(QPoint));
    }

    virtual void drawLine(const QPointF &p1, const QPointF &p2)
    {
        buffer.WriteUnsignedChar(F_drawLine_4);
        buffer.Write(&p1, sizeof(QPointF));
        buffer.Write(&p2, sizeof(QPointF));
    }


    virtual void drawRect(const QRectF &rect)
    {
        buffer.WriteUnsignedChar(F_drawRect_0);
        buffer.Write(&rect, sizeof(QRectF));
    }

    virtual void drawRect(int x1, int y1, int w, int h)
    {
        buffer.WriteUnsignedChar(F_drawRect_1);
        buffer.WriteInt(x1);
        buffer.WriteInt(y1);
        buffer.WriteInt(w);
        buffer.WriteInt(h);
    }

    virtual void drawRect(const QRect &rect)
    {
        buffer.WriteUnsignedChar(F_drawRect_2);
        buffer.Write(&rect, sizeof(QRect));
    }


    virtual void drawEllipse(const QRectF &r)
    {
        buffer.WriteUnsignedChar(F_drawEllipse_0);
        buffer.Write(&r, sizeof(QRectF));
    }

    virtual void drawEllipse(const QRect &r)
    {
        buffer.WriteUnsignedChar(F_drawEllipse_1);
        buffer.Write(&r, sizeof(QRect));
    }

    virtual void drawEllipse(int x, int y, int w, int h)
    {
        buffer.WriteUnsignedChar(F_drawEllipse_2);
        buffer.WriteInt(x);
        buffer.WriteInt(y);
        buffer.WriteInt(w);
        buffer.WriteInt(h);
    }


    virtual void drawEllipse(const QPointF &center, qreal rx, qreal ry)
    {
        buffer.WriteUnsignedChar(F_drawEllipse_3);
        buffer.Write(&center, sizeof(QPointF));
        buffer.WriteQReal(rx);
        buffer.WriteQReal(ry);
    }

    virtual void drawEllipse(const QPoint &center, int rx, int ry)
    {
        buffer.WriteUnsignedChar(F_drawEllipse_4);
        buffer.Write(&center, sizeof(QPoint));
        buffer.WriteInt(rx);
        buffer.WriteInt(ry);
    }


    virtual void drawImage(int x, int y, const QImage &image)
    {
        buffer.WriteUnsignedChar(F_drawImage);
        buffer.WriteInt(x);
        buffer.WriteInt(y);
        buffer.WritePtr(&image);
    }


    virtual void drawText(int x, int y, const QString &s)
    {
        buffer.WriteUnsignedChar(F_drawText);
        buffer.WriteInt(x);
        buffer.WriteInt(y);
        buffer.WritePtr(new QString(s));
    }


    virtual void setRenderHint(QPainter::RenderHint hint, bool on)
    {
        buffer.WriteUnsignedChar(F_setRenderHint);
        buffer.WriteUnsignedChar(hint);
        buffer.WriteBool(on);
    }

    virtual void setRenderHints(QPainter::RenderHints hints, bool on)
    {
        buffer.WriteUnsignedChar(F_setRenderHints);
        buffer.WriteUnsignedChar(hints);
        buffer.WriteBool(on);
    }

};

FORCEINLINE void QueuedPainter::ExecuteOnDevice(IPainter &painter)
{
    Q_ASSERT(buffer.GetReadPosition() == 0);
    Q_ASSERT(buffer.GetWritePosition() > 0);

    for (int cmd = buffer.ReadUnsignedChar();
         cmd != F_END;
         cmd = buffer.ReadUnsignedChar())
    {
        switch (cmd)
        {
            default:
                qDebug() << "!! ExecuteOnDevice :: Unknown queued paint command:" << cmd;
                return;
            case F_setCompositionMode: DBGMSG_QUEUE("F_setCompositionMode");
                painter.setCompositionMode(
                            (QPainter::CompositionMode)buffer.ReadUnsignedChar());
                break;
            case F_setFont: DBGMSG_QUEUE("F_setFont");
            {
                QFont *font = buffer.ReadPtr<QFont*>();
                painter.setFont(*font);
                if (cleanup)
                    delete font;
                break;
            }
            case F_setPen_0: DBGMSG_QUEUE("F_setPen_0");
            {
                QColor color;
                buffer.Read(sizeof(QColor), &color);
                painter.setPen(color);
                break;
            }
            case F_setPen_1: DBGMSG_QUEUE("F_setPen_1");
            {
                QPen *pen = buffer.ReadPtr<QPen*>();
                DBGMSG_QUEUE(pen);
                painter.setPen(*pen);
                if (cleanup)
                    delete pen;
                break;
            }
            case F_setPen_2: DBGMSG_QUEUE("F_setPen_2");
                painter.setPen((Qt::PenStyle)buffer.ReadUnsignedChar());
                break;
            case F_setBrush_0: DBGMSG_QUEUE("F_setBrush_0");
            {
                QBrush *brush = buffer.ReadPtr<QBrush*>();
                painter.setBrush(*brush);
                if (cleanup)
                    delete brush;
                break;
            }
            case F_setBrush_1: DBGMSG_QUEUE("F_setBrush_1");
                painter.setBrush((Qt::BrushStyle)buffer.ReadUnsignedChar());
                break;
            case F_setOpacity: DBGMSG_QUEUE("F_setOpacity");
                painter.setOpacity(buffer.ReadQReal());
                break;
            case F_save: DBGMSG_QUEUE("F_save");
                painter.save();
                break;
            case F_restore: DBGMSG_QUEUE("F_restore");
                painter.restore();
                break;
            case F_scale: DBGMSG_QUEUE("F_scale");
            {
                qreal sx = buffer.ReadQReal();
                painter.scale(sx, buffer.ReadQReal());
                break;
            }
            case F_shear: DBGMSG_QUEUE("F_shear");
            {
                qreal sh = buffer.ReadQReal();
                painter.shear(sh, buffer.ReadQReal());
                break;
            }
            case F_rotate: DBGMSG_QUEUE("F_rotate");
                painter.rotate(buffer.ReadQReal());
                break;
            case F_translate_0: DBGMSG_QUEUE("F_translate_0");
            {
                QPointF p;
                buffer.Read(sizeof(QPointF), &p);
                painter.translate(p);
                break;
            }
            case F_translate_1: DBGMSG_QUEUE("F_translate_1");
            {
                QPoint p;
                buffer.Read(sizeof(QPoint), &p);
                painter.translate(p);
                break;
            }
            case F_translate_2: DBGMSG_QUEUE("F_translate_2");
            {
                qreal dx = buffer.ReadQReal();
                painter.translate(dx, buffer.ReadQReal());
                break;
            }
            case F_drawLine_0: DBGMSG_QUEUE("F_drawLine_0");
            {
                QLineF line;
                buffer.Read(sizeof(QLineF), &line);
                painter.drawLine(line);
                break;
            }
            case F_drawLine_1: DBGMSG_QUEUE("F_drawLine_1");
            {
                QLine line;
                buffer.Read(sizeof(QLine), &line);
                painter.drawLine(line);
                break;
            }
            case F_drawLine_2: DBGMSG_QUEUE("F_drawLine_2");
            {
                int a = buffer.ReadInt();
                int b = buffer.ReadInt();
                int c = buffer.ReadInt();
                painter.drawLine(a, b, c, buffer.ReadInt());
                break;
            }
            case F_drawLine_3: DBGMSG_QUEUE("F_drawLine_3");
            {
                QPoint p1, p2;
                buffer.Read(sizeof(QPoint), &p1);
                buffer.Read(sizeof(QPoint), &p2);
                painter.drawLine(p1, p2);
                break;
            }
            case F_drawLine_4: DBGMSG_QUEUE("F_drawLine_4");
            {
                QPointF p1, p2;
                buffer.Read(sizeof(QPointF), &p1);
                buffer.Read(sizeof(QPointF), &p2);
                painter.drawLine(p1, p2);
                break;
            }
            case F_drawRect_0: DBGMSG_QUEUE("F_drawRect_0");
            {
                QRectF r;
                buffer.Read(sizeof(QRectF), &r);
                painter.drawRect(r);
                break;
            }
            case F_drawRect_1: DBGMSG_QUEUE("F_drawRect_1");
            {
                int a = buffer.ReadInt();
                int b = buffer.ReadInt();
                int c = buffer.ReadInt();
                painter.drawRect(a, b, c, buffer.ReadInt());
                break;
            }
            case F_drawRect_2: DBGMSG_QUEUE("F_drawRect_2");
            {
                QRect r;
                buffer.Read(sizeof(QRect), &r);
                painter.drawRect(r);
                break;
            }
            case F_drawEllipse_0: DBGMSG_QUEUE("F_drawEllipse_0");
            {
                QRectF r;
                buffer.Read(sizeof(QRectF), &r);
                painter.drawEllipse(r);
                break;
            }
            case F_drawEllipse_1: DBGMSG_QUEUE("F_drawEllipse_1");
            {
                QRect r;
                buffer.Read(sizeof(QRect), &r);
                painter.drawEllipse(r);
                break;
            }
            case F_drawEllipse_2: DBGMSG_QUEUE("F_drawEllipse_2");
            {
                int a = buffer.ReadInt();
                int b = buffer.ReadInt();
                int c = buffer.ReadInt();
                painter.drawEllipse(a, b, c, buffer.ReadInt());
                break;
            }
            case F_drawEllipse_3: DBGMSG_QUEUE("F_drawEllipse_3");
            {
                QPointF center;
                buffer.Read(sizeof(QPointF), &center);
                qreal rx = buffer.ReadQReal();
                painter.drawEllipse(center, rx, buffer.ReadQReal());
                break;
            }
            case F_drawEllipse_4: DBGMSG_QUEUE("F_drawEllipse_4");
            {
                QPoint center;
                buffer.Read(sizeof(QPoint), &center);
                int rx = buffer.ReadInt();
                painter.drawEllipse(center, rx, buffer.ReadInt());
                break;
            }
            case F_drawImage: DBGMSG_QUEUE("F_drawImage");
            {
                int x = buffer.ReadInt();
                int y = buffer.ReadInt();
                QImage *img = buffer.ReadPtr<QImage*>();
                DBGMSG_QUEUE(img);
                painter.drawImage(x, y, *img);
                break;
            }
            case F_drawText: DBGMSG_QUEUE("F_drawText");
            {
                int x = buffer.ReadInt();
                int y = buffer.ReadInt();
                QString *text = buffer.ReadPtr<QString*>();
                painter.drawText(x, y, *text);
                if (cleanup)
                    delete text;
            }
            case F_setRenderHint: DBGMSG_QUEUE("F_setRenderHint");
            {
                QPainter::RenderHint hint = (QPainter::RenderHint)buffer.ReadUnsignedChar();
                painter.setRenderHint(hint, buffer.ReadBool());
                break;
            }
            case F_setRenderHints: DBGMSG_QUEUE("F_setRenderHints");
            {
                QPainter::RenderHint hint = (QPainter::RenderHint)buffer.ReadUnsignedChar();
                painter.setRenderHints(hint, buffer.ReadBool());
                break;
            }
        }
    }
}

FORCEINLINE void QueuedPainter::Flush()
{
    DBGMSG_QUEUE("Flush :: read:" << buffer.GetReadPosition() << "write:" << buffer.GetWritePosition());

    Q_ASSERT(buffer.GetReadPosition() == 0 || buffer.GetReadPosition() == buffer.GetWritePosition());

    if (buffer.GetReadPosition() == buffer.GetWritePosition())
    {
        buffer.Reset();
        return;
    }

    for (int cmd = buffer.ReadUnsignedChar();
         cmd != F_END;
         cmd = buffer.ReadUnsignedChar())
    {
        switch (cmd)
        {
            default:
                qDebug() << "!! Flush :: Unknown queued paint command:" << cmd;
                return;
            case F_setCompositionMode: DBGMSG_QUEUE("Flush::F_setCompositionMode");
                buffer.FlushUnsignedChar();
                break;
            case F_setFont: DBGMSG_QUEUE("Flush::F_setFont");
                delete buffer.ReadPtr<QFont*>();
                break;
            case F_setPen_0: DBGMSG_QUEUE("Flush::F_setPen_0");
                buffer.Flush(sizeof(QColor));
                break;
            case F_setPen_1: DBGMSG_QUEUE("Flush::F_setPen_1");
                delete buffer.ReadPtr<QPen*>();
                break;
            case F_setPen_2: DBGMSG_QUEUE("Flush::F_setPen_2");
                buffer.FlushUnsignedChar();
                break;
            case F_setBrush_0: DBGMSG_QUEUE("Flush::F_setBrush_0");
                delete buffer.ReadPtr<QBrush*>();
                break;
            case F_setBrush_1: DBGMSG_QUEUE("Flush::F_setBrush_1");
                buffer.FlushUnsignedChar();
                break;
            case F_setOpacity: DBGMSG_QUEUE("Flush::F_setOpacity");
                buffer.FlushQReal();
                break;
            case F_save: DBGMSG_QUEUE("Flush::F_save");
                break;
            case F_restore: DBGMSG_QUEUE("Flush::F_restore");
                break;
            case F_scale: DBGMSG_QUEUE("Flush::F_scale");
                buffer.FlushQReal();
                buffer.FlushQReal();
                break;
            case F_shear: DBGMSG_QUEUE("Flush::F_shear");
                buffer.FlushQReal();
                buffer.FlushQReal();
                break;
            case F_rotate: DBGMSG_QUEUE("Flush::F_rotate");
                buffer.FlushQReal();
                break;
            case F_translate_0: DBGMSG_QUEUE("Flush::F_translate_0");
                buffer.Flush(sizeof(QPointF));
                break;
            case F_translate_1: DBGMSG_QUEUE("Flush::F_translate_1");
                buffer.Flush(sizeof(QPoint));
                break;
            case F_translate_2: DBGMSG_QUEUE("Flush::F_translate_2");
                buffer.FlushQReal();
                buffer.FlushQReal();
                break;
            case F_drawLine_0: DBGMSG_QUEUE("Flush::F_drawLine_0");
                buffer.Flush(sizeof(QLineF));
                break;
            case F_drawLine_1: DBGMSG_QUEUE("Flush::F_drawLine_1");
                buffer.Flush(sizeof(QLine));
                break;
            case F_drawLine_2: DBGMSG_QUEUE("Flush::F_drawLine_2");
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                break;
            case F_drawLine_3: DBGMSG_QUEUE("Flush::F_drawLine_3");
                buffer.Flush(sizeof(QPoint));
                buffer.Flush(sizeof(QPoint));
                break;
            case F_drawLine_4: DBGMSG_QUEUE("Flush::F_drawLine_4");
                buffer.Flush(sizeof(QPointF));
                buffer.Flush(sizeof(QPointF));
                break;
            case F_drawRect_0: DBGMSG_QUEUE("Flush::F_drawRect_0");
                buffer.Flush(sizeof(QRectF));
                break;
            case F_drawRect_1: DBGMSG_QUEUE("Flush::F_drawRect_1");
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                break;
            case F_drawRect_2: DBGMSG_QUEUE("Flush::F_drawRect_2");
                buffer.Flush(sizeof(QRect));
                break;
            case F_drawEllipse_0: DBGMSG_QUEUE("Flush::F_drawEllipse_0");
                buffer.Flush(sizeof(QRectF));
                break;
            case F_drawEllipse_1: DBGMSG_QUEUE("Flush::F_drawEllipse_1");
                buffer.Flush(sizeof(QRect));
                break;
            case F_drawEllipse_2: DBGMSG_QUEUE("Flush::F_drawEllipse_2");
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushInt();
                break;
            case F_drawEllipse_3: DBGMSG_QUEUE("Flush::F_drawEllipse_3");
                buffer.Flush(sizeof(QPointF));
                buffer.FlushQReal();
                buffer.FlushQReal();
                break;
            case F_drawEllipse_4: DBGMSG_QUEUE("Flush::F_drawEllipse_4");
                buffer.Flush(sizeof(QPoint));
                buffer.FlushInt();
                buffer.FlushInt();
                break;
            case F_drawImage: DBGMSG_QUEUE("Flush::F_drawImage");
                buffer.FlushInt();
                buffer.FlushInt();
                buffer.FlushPtr();
                break;
            case F_drawText: DBGMSG_QUEUE("Flush::F_drawText");
                buffer.FlushInt();
                buffer.FlushInt();
                delete buffer.ReadPtr<QString*>();
            case F_setRenderHint: DBGMSG_QUEUE("Flush::F_setRenderHint");
                buffer.FlushUnsignedChar();
                buffer.FlushBool();
                break;
            case F_setRenderHints: DBGMSG_QUEUE("Flush::F_setRenderHints");
                buffer.FlushUnsignedChar();
                buffer.FlushBool();
                break;
        }
    }

    Q_ASSERT(buffer.GetReadPosition() == buffer.GetWritePosition());

    buffer.Reset();
}

#endif // QUEUEDPAINTER_H
