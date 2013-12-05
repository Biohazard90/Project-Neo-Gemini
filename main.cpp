#include "mainwindow.h"
#include "main.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QFile>
#include <QTime>

#include "base.h"
#include "particlebase.h"
#include "gamebase.h"

#include "rootview.h"
#include "menubackground.h"
#include "simulationlist.h"
#include "precache.h"
#include "input.h"
#include "ranking.h"
#include "gameview.h"
#include "statistics.h"


MainWindow *mainWindow;
RootView *mainWidget;

void ResizeView(int w, int h)
{
    const bool isFullscreen = pGlobals->fullscreen;

    if (isFullscreen)
    {
        QRect desktop = QApplication::desktop()->screenGeometry();
        w = desktop.width();
        h = desktop.height();

        mainWindow->setWindowFlags((mainWindow->windowFlags()
                          | Qt::FramelessWindowHint)
                         & ~Qt::WindowMinMaxButtonsHint);
    }
    else
    {
        mainWindow->setWindowFlags(mainWindow->windowFlags() & ~(Qt::WindowMinMaxButtonsHint
                                                                 | Qt::FramelessWindowHint));
    }

    pGlobals->SetScreen(w, h);

    mainWindow->setFixedSize(w, h);
    //mainWindow->setGeometry(0, 0, w, h);

    //mainWidget->setFixedSize(w, h);
    mainWidget->setGeometry(0, 0, mainWindow->width(), mainWindow->height());

    if (isFullscreen)
    {
        mainWindow->showFullScreen();
    }
    else
    {
        mainWindow->show();

        CenterOnScreen(mainWindow);
    }
}

MainCleanup::MainCleanup(QWidget *){}
void MainCleanup::OnShutdown()
{
    Config::SaveConfig(pGlobals);

    DBGMSG("Shutdown cleanup");
    AudioManager::GetInstance()->Shutdown();
    ParticleFunctionFactory::GetInstance()->Shutdown();
}
static MainCleanup mainCleanup;

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().second() * 1000
                + QTime::currentTime().msec());

    qmlRegisterType<GameView>("CustomComponents", 1, 0, "GameView");

    Precache::GetInstance()->PrecacheAll();
    AudioManager::GetInstance()->Init();
    Input::GetInstance()->Init();

    Config::LoadConfig(pGlobals);
    Ranking::GetInstance()->LoadRanking();

    Resource::GetInstance()->Init();
    EntityFactory::GetInstance()->Init();
    AIFactory::GetInstance()->Init();
    Statistics::GetInstance()->Init();

    QApplication a(argc, argv);
    //QByteArray data = "1";
    //qputenv("QML_IMPORT_TRACE", data);

    mainWindow = new MainWindow();
    mainWidget = new RootView(mainWindow);
    a.installEventFilter(mainWindow);

    QTimer simulationTimer;
    simulationTimer.setInterval(DEFAULT_FRAMESPEED_MS);
    simulationTimer.setTimerType(Qt::CoarseTimer);

    QObject::connect(&simulationTimer,SIGNAL(timeout()),
                     SimulationList::GetInstance(), SLOT(OnFrame()));
    simulationTimer.start();
    
    mainWindow->setWindowTitle("Project Neo Gemini");
    //w.setAttribute(Qt::WA_TranslucentBackground);

#if 0
    mainWindow->setWindowFlags((mainWindow->windowFlags()
                      | Qt::FramelessWindowHint
                      | Qt::WindowStaysOnTopHint)
                     & ~Qt::WindowMinMaxButtonsHint);
#else
    mainWindow->setWindowFlags(mainWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
#endif

    int desiredWidth = MAX(640, pGlobals->screen_width);
    int desiredHeight = MAX(480, pGlobals->screen_height);

    ResizeView(desiredWidth, desiredHeight);

    QObject::connect(&a, SIGNAL(aboutToQuit()), &mainCleanup, SLOT(OnShutdown()));

    int ret = a.exec();

    Statistics::GetInstance()->Shutdown();

    delete mainWindow;

    return ret;
}
