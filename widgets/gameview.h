#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QDeclarativeItem>

class GameView : public QDeclarativeItem
{
    Q_OBJECT
public:
    GameView(QDeclarativeItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget);

signals:

public slots:

};

#endif // GAMEVIEW_H
