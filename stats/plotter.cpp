#include "../base.h"
#include "plotter.h"

#define FONT_FAMILY "calibri"

const int titleSize = 20;
const int titleMargin = 15;
const int imageMargin = 20;
const int imageMarginTop = titleSize + titleMargin * 2;

inline QString FormatString(const char *format, ...)
{
    char dest[1024] = { 0 };
    va_list args;
    va_start(args, format);
    _vsnprintf_s(dest, sizeof(dest), sizeof(dest), format, args);
    va_end(args);

    return dest;
}

inline void PaintTextCenteredW(int x, int y, QPainter &painter, const QString &text)
{
    int width = painter.fontMetrics().width(text);
    x -= width / 2;
    painter.drawText(x, y, text);
}

inline void PaintTextCenteredH(int x, int y, QPainter &painter, const QString &text)
{
    painter.drawText(x, y, text);
}

inline void PaintTextCenteredWH(int x, int y, QPainter &painter, const QString &text)
{
    x -= painter.fontMetrics().width(text) / 2;
    y += painter.fontMetrics().height() / 4;
    painter.drawText(x, y, text);
}

inline QRect GetTextRect(QPainter &painter, const QString &text)
{
    QRect rect(0, 0, painter.fontMetrics().width(text), painter.fontMetrics().height());

    return rect;
}

Plotter::Plotter(const QString &title, int width, int height)
    : image(width, height, QImage::Format_RGB32)
{
    this->width = width;
    this->height = height;
    this->title = title;

    image.fill(Qt::white);

    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    PaintTitle();
}

Plotter::~Plotter()
{

}

void Plotter::Clear()
{
    image.fill(Qt::white);
}

void Plotter::SaveTo(const QString &path)
{
    painter.end();

    image.save(PATH_GRAPHS_ROOT "/" + path + ".png");
}

void Plotter::PlotTimeLine(float minTime, float maxTime, QVector<float> &values)
{
    const int legendMargin = 20;

    painter.setClipRect(imageMargin, imageMarginTop + 25, width - imageMargin * 2,
                        height - imageMargin - legendMargin - imageMarginTop - 26);

    QColor brushColor(255, 0, 0, 0);
    brushColor.setAlphaF(qMin(1.0f, 3.0f / values.length()));

    for (auto &v : values)
    {
        float fraction = (v - minTime) / (maxTime - minTime);
        int xPos = imageMargin + fraction * (width - imageMargin * 2);

        painter.fillRect(xPos - 5, imageMarginTop, 10, height, brushColor);
    }

    painter.setClipping(false);

    painter.setPen(Qt::darkGray);
    painter.drawLine(imageMargin, imageMarginTop, imageMargin, height - imageMargin - legendMargin);
    painter.drawLine(imageMargin, height - imageMargin - legendMargin,
                     width - imageMargin, height - imageMargin - legendMargin);

    PaintLegend(imageMargin, height - imageMargin - legendMargin,
                width - imageMargin, height - imageMargin - legendMargin, values, 15);
}

void Plotter::PlotPieChart(QVector<float> &distributions, QVector<QString> &labels)
{
    if (distributions.length() != labels.length())
    {
        Q_ASSERT(0);
        return;
    }

    const int inset = 15;

    QRect rectSrc = GetPaintRect(true);
    rectSrc.adjust(inset, inset, inset * -2, inset * -2);

    Vector2D center(rectSrc.x() + rectSrc.width() * 0.5f,
                    rectSrc.y() + rectSrc.height() * 0.5f);

    float fullLength = 0.0f;

    for (auto l : distributions)
    {
        fullLength += l;
    }

    float currentPos = 0.0f;

    painter.setFont(QFont(FONT_FAMILY, 12));

    QVector<Vector2D> textPositions;

    for (int i = 0; i < distributions.length(); i++)
    {
        QColor col = QColor::fromHsv((i * 60) % 360,
                                    255 - 50 * (i / 360),
                                    255 - 30 * (i / 360));
        painter.setBrush(col);

        col.setRedF(col.redF() * 0.5f);
        col.setGreenF(col.greenF() * 0.5f);
        col.setBlueF(col.blueF() * 0.5f);
        painter.setPen(col);

        float degrees = distributions[i] / fullLength * 360.0f;
        int currentLength = 16 * degrees;

        float rad = DEG2RAD(currentPos + degrees * 0.5f);
        Vector2D dir( cos(rad), -sin(rad) );

        Vector2D textCenter = center + dir * width * 0.3f;

        QRect rect = rectSrc;

        dir *= 15;

        rect.translate(dir.x, dir.y);

        painter.drawPie(rect, currentPos * 16, currentLength);

        textPositions.append(textCenter);

        currentPos += degrees;
    }

    for (int i = 0; i < distributions.length(); i++)
    {
        const Vector2D &textCenter = textPositions[i];

        QString tag = labels[i] + FormatString(" %.1f%%", (distributions[i] / fullLength * 100.0f));

        QRect textRect = GetTextRect(painter, tag);
        textRect.moveCenter(textCenter.AsQPoint());

        painter.fillRect(textRect, QColor(255, 255, 255, 196));

        painter.setPen(Qt::black);
        PaintTextCenteredWH(textCenter.x, textCenter.y, painter, tag);
    }
}

QRect Plotter::GetPaintRect(bool squared)
{
    int w = width - imageMargin * 2;
    int h = height - imageMargin - imageMarginTop;

    if (squared)
    {
        w = qMin(w, h);
        h = qMin(w, h);
    }

    QRect rect;
    rect.setX((width - w) / 2);
    rect.setY(height - imageMargin - h);
    rect.setWidth(w);
    rect.setHeight(h);
    return rect;
}

void Plotter::PaintTitle()
{
    painter.setFont(QFont(FONT_FAMILY, titleSize));
    painter.setPen(Qt::gray);
    painter.drawText(imageMargin, titleMargin + titleSize, title);
}

void Plotter::PaintLegend(int x0, int y0, int x1, int y1,
                 QVector<QString> &values)
{
    const bool isHorizontal = abs(x0 - x1) > abs(y0 - y1);

//    painter.setPen(Qt::darkGray);
//    painter.drawLine(x0, y0, x0, y1);
//    painter.drawLine(x0, y1, x1, y1);

    QFont font(FONT_FAMILY, 10);

    painter.setPen(Qt::black);
    painter.setFont(font);

    Vector2D legendStart, legendEnd;

    if (isHorizontal)
    {
        legendStart.Init(x0, y1);
        legendEnd.Init(x1, y1);
    }
    else
    {
        legendStart.Init(x0, y1);
        legendEnd.Init(x0, y0);
    }

    Vector2D legendStep = (legendEnd - legendStart) / (values.length() - 1);
    Vector2D dashOffset = isHorizontal ? Vector2D(0.0f, 1.0f) : Vector2D(-1.0f, 0.0f);
    Vector2D textOffset = dashOffset * 25.0f;

    if (isHorizontal)
    {
        PaintTextCenteredW(legendStart.x + textOffset.x, legendStart.y + textOffset.y,
                           painter, values.first());
        PaintTextCenteredW(legendEnd.x + textOffset.x, legendEnd.y + textOffset.y,
                           painter, values.last());
    }
    else
    {
        PaintTextCenteredH(legendStart.x + textOffset.x, legendStart.y + textOffset.y,
                           painter, values.first());
        PaintTextCenteredH(legendEnd.x + textOffset.x, legendEnd.y + textOffset.y,
                           painter, values.last());
    }

    for (int i = 1; i < values.length() - 1; i++)
    {
        if (isHorizontal)
        {
            PaintTextCenteredW(legendStart.x + legendStep.x * i + textOffset.x,
                             legendStart.y + legendStep.y * i + textOffset.y,
                             painter, values.at(i));
        }
        else
        {
            PaintTextCenteredH(legendStart.x + legendStep.x * i + textOffset.x,
                             legendStart.y + legendStep.y * i + textOffset.y,
                             painter, values.at(i));
        }
    }

    painter.setPen(Qt::darkGray);

    for (int i = 0; i < values.length(); i++)
    {
        Vector2D pos = legendStart + legendStep * i;

        if (i == values.length() - 1)
            pos = legendEnd;

        pos += dashOffset;
        painter.drawLine(pos.x, pos.y, pos.x + dashOffset.x * 7.0f, pos.y + dashOffset.y * 7.0f);
    }
}

void Plotter::PaintLegend(int x0, int y0, int x1, int y1,
                          QVector<float> values, int stepCount, bool startAtZero)
{
    if (startAtZero)
    {
        values.append(0.0f);
    }

    float valueMin = values.first();
    float valueMax = values.last();

    for (auto v : values)
    {
        valueMin = qMin(valueMin, v);
        valueMax = qMax(valueMax, v);
    }

    const float stepSize = (valueMax - valueMin) / stepCount;
    float currentStep = valueMin + stepSize;

    QVector<QString> strings;

    strings.append(FormatString("%.1f", valueMin));

    for (int i = 1; i < stepCount; i++)
    {
        strings.append(FormatString("%.1f", currentStep));

        currentStep += stepSize;
    }

    strings.append(FormatString("%.1f", valueMax));

    PaintLegend(x0, y0, x1, y1, strings);
}
