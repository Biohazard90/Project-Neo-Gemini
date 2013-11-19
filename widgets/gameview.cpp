#include "gameview.h"
#include "particleview.h"

#include "gamebase.h"

#include "game.h"
#include "passthroughpainter.h"

GameView::GameView(QWidget *parent) :
    QWidget(parent)
{
    game = new Game(this);

    SimulationList::GetInstance()->AddSimulationObject(this);

    setFocusPolicy(Qt::StrongFocus);
}

GameView::~GameView()
{
    SimulationList::GetInstance()->RemoveSimulationObject(this);
}

Game *GameView::GetGame()
{
    return game;
}

void GameView::OnSimulate(float frametime)
{
    game->OnSimulate(frametime);

    update();
}

void GameView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    PassthroughPainter passthrough(painter);

    render_context_t c;
    c.painter = &passthrough;

    //c.painter = &painter;
    /*
    c.x = x() - 1;
    c.y = y() - 1;
    c.w = width() + 1;
    c.h = height() + 1;
    */

    c.x = 0;
    c.y = 0;
    c.w = width();
    c.h = height();

    game->OnRender(c);
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    Input::GetInstance()->OnKeyPressed(event);

    BaseClass::keyPressEvent(event);
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    Input::GetInstance()->OnKeyReleased(event);

    BaseClass::keyReleaseEvent(event);
}
