#ifndef PLOTTER_H
#define PLOTTER_H

class Plotter
{
public:
    Plotter(const QString &title, int width, int height);
    ~Plotter();

    void Clear();
    void SaveTo(const QString &path);

    void PlotTimeLine(float minTime, float maxTime, QVector<float> &values);
    void PlotPieChart(QVector<float> &distributions, QVector<QString> &labels);
    void PlotBarChart(QVector<float> &values, QVector<QString> &labels);

private:

    QRect GetPaintRect(bool squared);

    void PaintTitle();
    void PaintLegend(int x0, int y0, int x1, int y1,
                     QVector<QString> &values);
    void PaintLegend(int x0, int y0, int x1, int y1,
                     QVector<float> values, int stepCount, bool startAtZero = true);

    QImage image;
    int width, height;

    QString title;
    QPainter painter;
};

#endif // PLOTTER_H
