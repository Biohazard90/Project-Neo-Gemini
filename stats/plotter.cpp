#include "../base.h"
#include "plotter.h"

#define FONT_FAMILY "calibri"

const int titleSize = 20;
const int titleMargin = 15;
const int imageMargin = 20;
const int imageMarginTop = titleSize + titleMargin * 2;

QString FormatString(const char *format, ...)
{
    char dest[1024] = { 0 };
    va_list args;
    va_start(args, format);
    _vsnprintf_s(dest, sizeof(dest), sizeof(dest), format, args);
    va_end(args);

    return dest;
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
    const int stepCount = 10;

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

    painter.setPen(Qt::black);
    painter.setFont(QFont(FONT_FAMILY, 10));

    const int legendStart = imageMargin;
    const int legendEnd = width - imageMargin;
    const int legendStep = float(legendEnd - legendStart) / stepCount;

    painter.drawText(legendStart - 4, height - imageMargin, QString("%1").arg(minTime));
    painter.drawText(legendEnd - 8, height - imageMargin, FormatString("%.1f", maxTime));

    const float stepSize = (maxTime - minTime) / stepCount;
    float currentStep = minTime + stepSize;

    for (int i = 1; i < stepCount; i++)
    {
        painter.drawText(legendStart + legendStep * i - 8, height - imageMargin,
                         FormatString("%.1f", currentStep));

        currentStep += stepSize;
    }

    painter.setPen(Qt::darkGray);

    for (int i = 0; i <= stepCount; i++)
    {
        int xPos = legendStart + legendStep * i;
        int yPos = height - imageMargin - legendMargin;

        if (i == stepCount)
            xPos = legendEnd;

        painter.drawLine(xPos, yPos + 1, xPos, yPos + 7);
    }
}

void Plotter::PaintTitle()
{
    painter.setFont(QFont(FONT_FAMILY, titleSize));
    painter.setPen(Qt::gray);
    painter.drawText(imageMargin, titleMargin + titleSize, title);
}
