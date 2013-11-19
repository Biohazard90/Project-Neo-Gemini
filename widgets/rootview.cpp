#include "rootview.h"

#include "main.h"
#include "menubackground.h"
#include "particlebase.h"
#include "particleview.h"
#include "gameview.h"
#include "game.h"
#include "score.h"
#include "ranking.h"

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGraphicsObject>
#include <QKeyEvent>

#include <QDir>


RootView::RootView(QWidget *parent) :
    QWidget(parent)
{
    AAEnabled = pGlobals->antialiasing;
    ParallelRenderingEnabled = pGlobals->parallelrendering;
    FPSEnabled = pGlobals->showfps;
    MusicEnabled = pGlobals->musicenabled;

    background = NULL;
    particleView = NULL;
    gameView = NULL;
    mainQml = NULL;
    hudQml = NULL;

    setFocusPolicy(Qt::StrongFocus);

    ShowBackground();
    ShowMenu();
}

RootView::~RootView()
{
}

void RootView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int w = e->size().width();
    int h = e->size().height();

    if (background != NULL)
        background->setFixedSize(w, h);
    if (particleView != NULL)
        particleView->setFixedSize(w, h);
    if (mainQml != NULL)
        mainQml->setFixedSize(w, h);
}

void RootView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        EscapePressed();
        break;
    case Qt::Key_H:
        ShowGameOver();
        break;
    case Qt::Key_P:
    {
        if (gameView != NULL)
            gameView->GetGame()->GetParticleRoot()->CreateParticles("explosion_fighter_3_hit", Vector2D(320, 0), Vector2D(1, 0));
        break;
    }
    default:
        BaseClass::keyPressEvent(event);
        break;
    }
}

void RootView::EscapePressed()
{
    if (gameView != NULL
            && mainQml == NULL)
    {
        ShowMenu(MENU_Ingame);
    }
}

void RootView::ShowBackground()
{
    if (background != NULL)
        background->deleteLater();
    if (particleView != NULL)
        particleView->deleteLater();

    background = new MenuBackground(this);
    particleView = new ParticleView(this);
    particleView->GetParticleRoot()->CreateParticles("menu", Vector2D(320,240));

    background->setFixedSize(width(), height());
    particleView->setFixedSize(width(), height());

    background->show();
    particleView->show();

    if (mainQml != NULL)
        mainQml->raise();

    AudioManager::GetInstance()->PlayMusic("menu.ogg");
}

void RootView::HideBackground()
{
    if (background != NULL)
        background->deleteLater();
    if (particleView != NULL)
        particleView->deleteLater();

    background = NULL;
    particleView = NULL;
}

void RootView::ShowMenu(MenuMode_e mode)
{
    if (gameView != NULL)
        gameView->GetGame()->SetPaused(true);

    if (mainQml != NULL)
        mainQml->deleteLater();

    mainQml = new QDeclarativeView(this);
    mainQml->setStyleSheet(QString("background: transparent"));
    mainQml->rootContext()->setContextProperty("menuController", this);
    mainQml->rootContext()->setContextProperty("scoreController", Score::GetInstance());
    mainQml->rootContext()->setContextProperty("rankingController", Ranking::GetInstance());
    mainQml->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    mainQml->setSource(QUrl::fromLocalFile(OSLocalPath("qml/Mainmenu.qml")));

    QObject::connect(mainQml->engine(), SIGNAL(quit()), parent(), SLOT(close()));

    mainQml->setFixedSize(width(), height());

    switch (mode)
    {
    case MENU_Ingame:
        emit prepareIngameMenu();
        break;
    case MENU_Gameover:
        emit prepareGameoverMenu();
        break;
    }

    mainQml->show();

    Ranking::GetInstance()->Update();
}

void RootView::HideMenu()
{
    if (mainQml != NULL)
        mainQml->deleteLater();

    mainQml = NULL;

    if (gameView != NULL)
        gameView->setFocus();
    else
        setFocus();
}

void RootView::ShowGameOver()
{
    if (gameView != NULL)
    {
        ShowMenu(MENU_Gameover);
    }
}

void RootView::ShowScore()
{
    if (gameView != NULL
            && mainQml == NULL)
    {
        ShowMenu(MENU_Score);
    }
}

void RootView::CreateGame(const char *mapname, bool newGame)
{
    if (newGame)
        Score::GetInstance()->Reset();

    if (hudQml != NULL)
        hudQml->deleteLater();

    if (gameView != NULL)
        gameView->deleteLater();

    gameView = new GameView(this);
    QObject::connect(gameView->GetGame(), SIGNAL(GameEnded()), this, SLOT(onGameOver()));

    gameView->setGeometry(geometry());

    gameView->GetGame()->LoadMap(mapname);

    gameView->show();

    hudQml = new QDeclarativeView(this);
    hudQml->setStyleSheet(QString("background: transparent"));
    hudQml->rootContext()->setContextProperty("menuController", this);
    hudQml->rootContext()->setContextProperty("scoreController", Score::GetInstance());
    hudQml->rootContext()->setContextProperty("gameController", gameView->GetGame());
    hudQml->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    hudQml->setSource(QUrl::fromLocalFile(OSLocalPath("qml/Hud.qml")));
    hudQml->setFixedSize(width(), height());

    hudQml->show();

    hudQml->setAttribute(Qt::WA_TransparentForMouseEvents);

    if (mainQml != NULL)
        mainQml->raise();
}

void RootView::DestroyGame()
{
    if (hudQml != NULL)
        hudQml->deleteLater();

    if (gameView != NULL)
        gameView->deleteLater();

    hudQml = NULL;
    gameView = NULL;
}

void RootView::setAAEnabled(bool enabled)
{
    AAEnabled = enabled;
    pGlobals->antialiasing = enabled;
    emit AAEnabledChanged(enabled);
}

void RootView::setParallelRenderingEnabled(bool enabled)
{
    ParallelRenderingEnabled = enabled;
    pGlobals->parallelrendering = enabled;
    emit ParallelRenderingEnabledChanged(enabled);
}

void RootView::setFPSEnabled(bool enabled)
{
    FPSEnabled = enabled;
    pGlobals->showfps = enabled;
    emit FPSEnabledChanged(enabled);
}

void RootView::setMusicEnabled(bool enabled)
{
    MusicEnabled = enabled;
    pGlobals->musicenabled = enabled;
    AudioManager::GetInstance()->SetMusicEnabled(enabled);
    emit MusicEnabledChanged(enabled);
}

void RootView::setLevelName(QString name)
{
    name.replace('_', ' ');
    LevelName = name;
    emit LevelNameChanged(name);
}

void RootView::onButtonHover(QGraphicsObject *caller, bool containsMouse)
{
    if (containsMouse)
        AudioManager::GetInstance()->PlaySound("menu/rollover_1.wav");
}

void RootView::onButtonClick()
{
    AudioManager::GetInstance()->PlaySound("menu/rollover_2.wav");
}

void RootView::onStartGame(QString mapname, bool newGame)
{
    DBGMSG("Map start:" << mapname);

    std::string strMapname = mapname.toLower().toStdString();

    HideBackground();
    CreateGame(strMapname.c_str(), newGame);
}

void RootView::onSetResolution(QString key)
{
    DBGMSG("Change resolution:" << key);
    QStringList vars = key.split('x');

    if (vars.size() != 2)
        return;

    ResizeView(vars.at(0).toInt(), vars.at(1).toInt());
}

void RootView::onMenuFadedOut()
{
    HideMenu();

    if (gameView != NULL)
        gameView->GetGame()->SetPaused(false);
}

void RootView::onShowBackground()
{
    DestroyGame();
    ShowBackground();
}

void RootView::onLevelIntro()
{
    AudioManager::GetInstance()->PlayMusic("intro.ogg", 1.0f, false);
}

void RootView::onGameOver()
{
    ShowGameOver();
}
