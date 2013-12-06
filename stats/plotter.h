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

private:

    void PaintTitle();

    QImage image;
    int width, height;

    QString title;
    QPainter painter;
};

#endif // PLOTTER_H
