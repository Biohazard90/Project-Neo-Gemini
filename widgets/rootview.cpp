#include "rootview.h"

#include "main.h"
#include "menubackground.h"
#include "particlebase.h"
#include "particleview.h"
#include "gamebase.h"
#include "game.h"
#include "score.h"
#include "ranking.h"

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGraphicsObject>
#include <QKeyEvent>

#include <QDir>

RootView *VIEW;

RootView::RootView(QWidget *parent) :
    BaseClass(parent)
  , GameVisible(false)
{
    VIEW = this;

    AAEnabled = pGlobals->antialiasing;
    FPSEnabled = pGlobals->showfps;
    MusicEnabled = pGlobals->musicenabled;
    FullscreenEnabled = pGlobals->fullscreen;

    background = NULL;
    particleView = NULL;
    game = NULL;
    mainQml = NULL;

    setFocusPolicy(Qt::StrongFocus);

    SimulationList::GetInstance()->AddSimulationObject(this);

    ShowBackground();
    ShowMenu();

    //CreateGame("space", true);
}

RootView::~RootView()
{
    SimulationList::GetInstance()->RemoveSimulationObject(this);
}

void RootView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int w = e->size().width();
    int h = e->size().height();

    if (mainQml != NULL)
        mainQml->setFixedSize(w, h);
    //mainQml->setViewport(this);

    //setFixedSize(w, h);
}

void RootView::OnSimulate(float frametime)
{
    if (game != NULL)
    {
        game->OnSimulate(frametime);
    }
}

void RootView::externalPaintEvent(render_context_t &context)
{
    if (game != NULL)
    {
        game->OnRender(context);
    }

    if (background != NULL)
    {
        background->paintEvent(context);
    }

    if (particleView != NULL)
    {
        particleView->paintEvent(context);
    }
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
            if (game != NULL)
                game->GetParticleRoot()->CreateParticles("player_shield_damage", Vector2D(320, 0), Vector2D(1, 0));
        }
        break;
    default:
        {
            if (game != NULL)
                Input::GetInstance()->OnKeyPressed(event);

            BaseClass::keyPressEvent(event);
        }
        break;
    }
}

void RootView::keyReleaseEvent(QKeyEvent *event)
{
    if (game != NULL)
        Input::GetInstance()->OnKeyReleased(event);

    BaseClass::keyReleaseEvent(event);
}

void RootView::EscapePressed()
{
    if (game != NULL)
    {
        mainQml->setFocus();
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
    if (game != NULL)
        game->SetPaused(true);

    if (mainQml == NULL)
    {
        QGLWidget *glTarget = new QGLWidget(this);
        glTarget->setFormat(QGLFormat(QGL::SampleBuffers));

        mainQml = new QDeclarativeView(this);
        mainQml->setViewport(glTarget);
        mainQml->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        mainQml->rootContext()->setContextProperty("menuController", this);
        mainQml->rootContext()->setContextProperty("scoreController", Score::GetInstance());
        mainQml->rootContext()->setContextProperty("rankingController", Ranking::GetInstance());
        mainQml->setResizeMode(QDeclarativeView::SizeRootObjectToView);

        mainQml->setSource(QUrl::fromLocalFile(OSLocalPath("qml/Mainmenu.qml")));

        QObject::connect(mainQml->engine(), SIGNAL(quit()), parent(), SLOT(close()));

        mainQml->setFixedSize(width(), height());
        mainQml->show();
    }

    mainQml->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    mainQml->setFocusPolicy(Qt::StrongFocus);

    switch (mode)
    {
    case MENU_Ingame:
        emit prepareIngameMenu();
        break;
    case MENU_Gameover:
        emit prepareGameoverMenu();
        break;
    }

    Ranking::GetInstance()->Update();
}

void RootView::HideMenu()
{
    if (mainQml != NULL)
    {
        mainQml->setFocusPolicy(Qt::NoFocus);
        mainQml->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    setFocus();
}

void RootView::ShowGameOver()
{
    if (game != NULL)
    {
        ShowMenu(MENU_Gameover);
    }
}

void RootView::ShowScore()
{
    if (game != NULL
            && mainQml == NULL)
    {
        ShowMenu(MENU_Score);
    }
}

void RootView::CreateGame(const char *mapname, bool newGame)
{
    if (newGame)
        Score::GetInstance()->Reset();

    if (game != NULL)
        game->deleteLater();

    game = new Game(this);
    QObject::connect(game, SIGNAL(GameEnded()), this, SLOT(onGameOver()));

   // gameView->setGeometry(geometry());
    //gameView->setGeometry(0, 0, 1920, 1080);

    game->LoadMap(mapname);
    mainQml->rootContext()->setContextProperty("gameController", game);
    setGameVisible(true);


   // hudQml->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void RootView::DestroyGame()
{
    setGameVisible(false);

    if (game != NULL)
        game->deleteLater();

    game = NULL;
}

void RootView::setAAEnabled(bool enabled)
{
    AAEnabled = enabled;
    pGlobals->antialiasing = enabled;
    emit AAEnabledChanged(enabled);
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

void RootView::setGameVisible(bool visible)
{
    GameVisible = visible;
    emit GameVisibleChanged(visible);
}

void RootView::setFullscreenEnabled(bool enabled)
{
    FullscreenEnabled = enabled;
    pGlobals->fullscreen = enabled;
    emit FullscreenEnabledChanged(enabled);

    ResizeView(pGlobals->screen_width, pGlobals->screen_height);
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
        AudioManager::GetInstance()->PlaySoundSample("menu/rollover_1.wav");
}

void RootView::onButtonClick()
{
    AudioManager::GetInstance()->PlaySoundSample("menu/rollover_2.wav");
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

    if (game != NULL)
        game->SetPaused(false);
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
