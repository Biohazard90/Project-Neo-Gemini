#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

#include "macros.h"
#include "isimulated.h"

class Game;

class GameView : public QWidget, ISimulated
{
    DECLARE_CLASS(GameView, QWidget);
    Q_OBJECT
public:
    explicit GameView(QWidget *parent = 0);
    ~GameView();

    virtual void OnSimulate(float frametime);
    virtual void paintEvent(QPaintEvent *);

    Game *GetGame();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
signals:
    
public slots:

private:
    Game *game;
    
};

#endif // GAMEVIEW_H
