#include "menubackground.h"
#include "simulationlist.h"
#include "base.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QResizeEvent>
#include <QBrushData>
#include <QCursor>

MenuBackground::MenuBackground(QWidget *parent) :
    QObject(parent)
{
    memset(m_flPanels, 0, sizeof(m_flPanels));

    SimulationList::AddSimulationObject(this);
}

MenuBackground::~MenuBackground()
{
    SimulationList::RemoveSimulationObject(this);
}

void MenuBackground::paintEvent(render_context_t &context)
{
    QPainter &painter = *context.painter;

    const float flStart = VALUE_PROPORTIONAL(50);
    const float flDelta = VALUE_PROPORTIONAL(180);

    QColor backgroundInner(30, 50, 90, 255);
    QColor backgroundOuter(10, 18, 35, 255);

    int w2 = context.w / 2;
    int h2 = context.h / 2;

    Vector2D cursor = Vector2D(context.mx, context.my)
            - Vector2D(w2,h2);
    cursor *= -1;

    cursor.x = CLAMP(cursor.x, -w2, w2);
    cursor.y = CLAMP(cursor.y, -h2, h2);

    Vector2D bgPos(cursor);
    bgPos *= 0.02f;
    bgPos.x += context.w * 0.5f;
    bgPos.y += context.h * 0.5f;

    QRadialGradient gradient(bgPos.AsQPointF(), context.w * 0.5f);
    gradient.setColorAt(0.0, backgroundInner);
    gradient.setColorAt(1.0, backgroundOuter);

    painter.setBrush(QBrush(gradient));
    painter.drawRect(context.x, context.y, context.w, context.h);

    cursor *= 2.0f;

    painter.setRenderHints(QPainter::Antialiasing, pGlobals->antialiasing);

    Vector2D cursorOffset(cursor);
    cursorOffset *= 0.05f;

    painter.beginNativePainting();

    glPopAttrib();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    for (int i = 0; i < QARRAYSIZE(m_flPanels); i++)
    {
        float flFraction = i/(float)(QARRAYSIZE(m_flPanels) - 1);
        flFraction = fmod(m_flPanels[i] + flFraction, 1.0f);
        const float flFractionSqr = sqr(flFraction) * flFraction;

        const float flAdjust = (1.0f - flFractionSqr) * flDelta;

        QRectF rect(flAdjust-flStart+cursorOffset.x*flFractionSqr,
                    flAdjust-flStart+cursorOffset.y*flFractionSqr,
                    context.w+flStart*2-flAdjust*2,
                    context.h+flStart*2-flAdjust*2);

//        QColor pencolor(220, 220, 220, 128 * flFractionSqr);
//        QPen pen(QBrush(pencolor),flFraction * 2.0f);
//        painter.setPen(pen);
//        painter.setBrush(Qt::transparent);
//        painter.drawRoundRect(rect,
//                              15 * pGlobals->screen_ratio_inv,
//                              15);

        glColor4f(0.9f, 0.9f, 0.9f, flFractionSqr * 0.5f);
        glLineWidth(flFraction * 2.0f);
        DrawGLRoundedRect(rect, 0.06f + 0.04f * flFractionSqr);
    }

    glPushAttrib(GL_ENABLE_BIT);

    painter.endNativePainting();

    const Vector2D dirs[] = {
        Vector2D( 1, 1 ),
        Vector2D( -1, 1 ),
        Vector2D( -1, -1 ),
        Vector2D( 1, -1 ),
        Vector2D( 0, 1 ),
        Vector2D( 0, -1 ),
        Vector2D( 1, 0 ),
        Vector2D( -1, 0 ),
    };

    const Vector2D offsets[] = {
        vec2_origin,
        Vector2D(context.w, 0),
        Vector2D(context.w, context.h),
        Vector2D(0, context.h),
        Vector2D(context.w*0.5f, 0),
        Vector2D(context.w*0.5f, context.h),
        Vector2D(0, context.h*0.5f),
        Vector2D(context.w, context.h*0.5f),
    };

    QLinearGradient g;
    g.setColorAt(1.0f, Qt::transparent);
    g.setColorAt(0, QColor(220, 220, 220, 128));

    for (int i = 0; i < QARRAYSIZE(dirs); i++)
    {
        const Vector2D &d = dirs[i];
        const Vector2D &o = offsets[i];

        Vector2D topLeft(d.x * -flStart,d.y * -flStart);
        Vector2D innerTopLeft(d.x * flDelta, d.y * flDelta);

        innerTopLeft += topLeft;

        innerTopLeft += o;
        topLeft += cursorOffset + o;

        g.setStart(topLeft.AsQPointF());
        g.setFinalStop(innerTopLeft.AsQPointF());
        QPen penLine(QBrush(g), 1.0f);
        painter.setPen(penLine);
        painter.drawLine(topLeft.AsQPointF(),innerTopLeft.AsQPointF());
    }
}

void MenuBackground::DrawGLRoundedRect(const QRectF &rect, float roundPerc)
{
    const float amt = roundPerc * rect.width();
    const int subDiv = 13;

    glPushMatrix();

    glTranslatef(rect.x(), rect.y(), 0.0f);

    glBegin(GL_LINE_LOOP);

    glVertex2f(amt, 0.0f);
    glVertex2f(rect.width() - amt, 0.0f);

    DrawGLRoundedCorner(Vector2D(rect.width() - amt, amt), -90.0f, 0.0f, amt, subDiv);

    glVertex2f(rect.width(), amt);
    glVertex2f(rect.width(), rect.height() - amt);

    DrawGLRoundedCorner(Vector2D(rect.width() - amt, rect.height() - amt), 0.0f, 90.0f, amt, subDiv);

    glVertex2f(rect.width() - amt, rect.height());
    glVertex2f(amt, rect.height());

    DrawGLRoundedCorner(Vector2D(amt, rect.height() - amt), 90.0f, 180.0f, amt, subDiv);

    glVertex2f(0.0f, rect.height() - amt);
    glVertex2f(0.0f, amt);

    DrawGLRoundedCorner(Vector2D(amt, amt), 180.0f, 270.0f, amt, subDiv);

    glEnd();

    glPopMatrix();
}

void MenuBackground::DrawGLRoundedCorner(const Vector2D &center, float startDegrees, float endDegrees, float size, int subDiv)
{
    float deltaDegrees = endDegrees - startDegrees;
    deltaDegrees /= (subDiv - 1);

    for (int i = 0; i < subDiv; i++)
    {
        Vector2D dir = Vector2D::AngleDirection(startDegrees) * size;
        dir += center;

        glVertex2f(dir.x, dir.y);

        startDegrees += deltaDegrees;
    }
}

void MenuBackground::OnSimulate(float frametime)
{
    for (int i = 0; i < QARRAYSIZE(m_flPanels); i++)
    {
        m_flPanels[i] += frametime * 0.25f;
        if (m_flPanels[i] > 1.0f)
            m_flPanels[i] = fmod(m_flPanels[i], 1.0f);
    }
}

