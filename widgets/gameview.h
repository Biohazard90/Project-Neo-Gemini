#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QDeclarativeItem>
#include <QElapsedTimer>

class GameView : public QDeclarativeItem
{
    Q_OBJECT
public:
    GameView(QDeclarativeItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget);

signals:

public slots:

private:
    QElapsedTimer timer;
    qint64 lastTime;

    float accumulatedTime;
    int framecount;
};

#endif // GAMEVIEW_H
