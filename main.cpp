#include "mainwindow.h"
#include "main.h"

#include <QApplication>
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


MainWindow *mainWindow;
RootView *mainWidget;

void ResizeView(int w, int h)
{
    pGlobals->SetScreen(w, h);

    mainWindow->setFixedSize(w,h);
    mainWidget->setFixedSize(w, h);
    mainWidget->setGeometry(0,0,w,h);

    //mainWidget->setMask(mainWidget->geometry());

    CenterOnScreen(mainWindow);
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

    QApplication a(argc, argv);
    //QByteArray data = "1";
    //qputenv("QML_IMPORT_TRACE", data);

    MainWindow w;
    mainWindow = &w;
    mainWidget = new RootView(&w);

    QTimer simulationTimer;
    simulationTimer.setInterval(DEFAULT_FRAMESPEED_MS);
    simulationTimer.setTimerType(Qt::CoarseTimer);

    QObject::connect(&simulationTimer,SIGNAL(timeout()),
                     SimulationList::GetInstance(), SLOT(OnFrame()));
    simulationTimer.start();
    
    w.setWindowTitle("Shazam");
    //w.setAttribute(Qt::WA_TranslucentBackground);

#if 0
    w.setWindowFlags((w.windowFlags()
                      | Qt::FramelessWindowHint
                      | Qt::WindowStaysOnTopHint)
                     & ~Qt::WindowMinMaxButtonsHint);
#else
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMinMaxButtonsHint);
#endif

    int desiredWidth = MAX(640, pGlobals->screen_width);
    int desiredHeight = MAX(480, pGlobals->screen_height);

    ResizeView(desiredWidth,desiredHeight);

    QObject::connect(&a, SIGNAL(aboutToQuit()), &mainCleanup, SLOT(OnShutdown()));
    w.show();
    CenterOnScreen(&w);

    int ret = a.exec();

    mainWidget = NULL;
    mainWindow = NULL;

    return ret;
}
