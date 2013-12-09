#include "rootview.h"

#include "main.h"
#include "menubackground.h"
#include "particlebase.h"
#include "particleview.h"
#include "gamebase.h"
#include "game.h"
#include "score.h"
#include "ranking.h"
#include "statistics.h"

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QMessageBox>

#include <QDir>

RootView *RootView::rootViewInstance = nullptr;

RootView::RootView(QWidget *parent) :
    BaseClass(parent)
  , GameVisible(false)
  , MouseX(0.0)
  , MouseY(0.0)
{
    rootViewInstance = this;

    AAEnabled = pGlobals->antialiasing;
    FPSEnabled = pGlobals->showfps;
    MusicEnabled = pGlobals->musicenabled;
    FullscreenEnabled = pGlobals->fullscreen;

    background = nullptr;
    particleView = nullptr;
    game = nullptr;
    mainQml = nullptr;

    setFocusPolicy(Qt::StrongFocus);

    SimulationList::GetInstance()->AddSimulationObject(this);

    ShowBackground();
    ShowMenu();

    //CreateGame("space", true);
}

RootView::~RootView()
{
    Q_ASSERT(rootViewInstance == this);
    if (rootViewInstance == this)
    {
        rootViewInstance = nullptr;
    }

    SimulationList::GetInstance()->RemoveSimulationObject(this);
}

RootView *RootView::GetActiveRootView()
{
    return rootViewInstance;
}

void RootView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int w = e->size().width();
    int h = e->size().height();

    if (mainQml != nullptr)
        mainQml->setFixedSize(w, h);
    //mainQml->setViewport(this);

    //setFixedSize(w, h);
}

void RootView::OnSimulate(float frametime)
{
    if (game != nullptr)
    {
        game->OnSimulate(frametime);
    }

    if (MouseX != pGlobals->mouse_x)
    {
        MouseX = pGlobals->mouse_x;

        emit MouseXChanged(MouseX);
    }

    if (MouseY != pGlobals->mouse_y)
    {
        MouseY = pGlobals->mouse_y;

        emit MouseYChanged(MouseY);
    }
}

void RootView::ExternalPaintEvent(render_context_t &context)
{
    if (game != nullptr)
    {
        game->OnRender(context);
    }

    if (background != nullptr)
    {
        background->paintEvent(context);
    }

    if (particleView != nullptr)
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
            if (game != nullptr)
                game->GetParticleRoot()->CreateParticles("player_shield_damage", Vector2D(320, 0), Vector2D(1, 0));
        }
        break;
    case Qt::Key_F12:
        {
            Statistics::GetInstance()->GenerateGraphs();
        }
        break;
    default:
        {
            if (game != nullptr
                    && Input::GetInstance()->OnKeyPressed(event))
                return;

            BaseClass::keyPressEvent(event);
        }
        break;
    }
}

void RootView::keyReleaseEvent(QKeyEvent *event)
{
    if (game != nullptr
            && Input::GetInstance()->OnKeyReleased(event))
        return;

    BaseClass::keyReleaseEvent(event);
}

void RootView::EscapePressed()
{
    if (game != nullptr)
    {
        mainQml->setFocus();
        ShowMenu(MENU_Ingame);
    }
}

void RootView::ShowBackground()
{
    if (background != nullptr)
        background->deleteLater();

    if (particleView != nullptr)
        particleView->deleteLater();

    background = new MenuBackground(this);
    particleView = new ParticleView(this);

    particleView->GetParticleRoot()->CreateParticles("menu", Vector2D(320,240));

    if (mainQml != nullptr)
        mainQml->raise();

    AudioManager::GetInstance()->PlayMusic("menu.ogg");
}

void RootView::HideBackground()
{
    if (background != nullptr)
        background->deleteLater();
    if (particleView != nullptr)
        particleView->deleteLater();

    background = nullptr;
    particleView = nullptr;
}

void RootView::ShowMenu(MenuMode_e mode)
{
    if (game != nullptr)
        game->SetPaused(true);

    if (mainQml == nullptr)
    {
        QGLWidget *glTarget = new QGLWidget(this);

        QGLFormat format;
        format.setSampleBuffers(true);
        format.setVersion(3, 1);
        format.setProfile(QGLFormat::CoreProfile);
        glTarget->setFormat(format);

        if (!glTarget->isValid())
        {
            QMessageBox::warning(this, "Error", "Unnable to init OpenGL!");
        }

        glPushAttrib(GL_ENABLE_BIT);

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

    mainQml->setAttribute(Qt::WA_OpaquePaintEvent, true);
    mainQml->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    mainQml->setFocusPolicy(Qt::StrongFocus);

    qApp->processEvents();

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

    setCursor(QCursor());
}

void RootView::HideMenu()
{
    if (mainQml != nullptr)
    {
        mainQml->setFocusPolicy(Qt::NoFocus);
        mainQml->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    setFocus();
}

void RootView::ShowGameOver()
{
    if (game != nullptr)
    {
        ShowMenu(MENU_Gameover);
    }
}

void RootView::ShowScore()
{
    if (game != nullptr
            && mainQml == nullptr)
    {
        ShowMenu(MENU_Score);
    }
}

void RootView::CreateGame(const char *mapname, bool newGame)
{
    if (newGame)
        Score::GetInstance()->Reset();

    if (game != nullptr)
        game->deleteLater();

    game = new Game(this);
    QObject::connect(game, SIGNAL(GameEnded()), this, SLOT(onGameOver()));

   // gameView->setGeometry(geometry());
    //gameView->setGeometry(0, 0, 1920, 1080);

    mainQml->rootContext()->setContextProperty("gameController", game);

    qApp->processEvents();

    game->LoadMap(mapname);
    setGameVisible(true);


    setCursor(QCursor(Qt::BlankCursor));
   // hudQml->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void RootView::DestroyGame()
{
    setGameVisible(false);

    if (game != nullptr)
        game->deleteLater();

    game = nullptr;
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

    if (game != nullptr)
        game->SetPaused(false);

    setCursor(QCursor(Qt::BlankCursor));
}

void RootView::onShowBackground()
{
    DestroyGame();
    ShowBackground();
}

void RootView::onAbortGame()
{
    if (game != nullptr)
    {
        KeyValues *event = new KeyValues("map_aborted");
        event->SetFloat("time", game->GetGameTime());

        if (game->GetPlayer() != nullptr)
        {
            event->SetInt("player_health", game->GetPlayer()->GetHealth());
        }

        Events::GetInstance()->FireEvent(event);
    }
}

void RootView::onLevelIntro()
{
    AudioManager::GetInstance()->PlayMusic("intro.ogg", 1.0f, false);
}

void RootView::onGameOver()
{
    ShowGameOver();
}
