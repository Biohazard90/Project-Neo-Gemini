#ifndef MENUROOT_H
#define MENUROOT_H

#include "macros.h"
#include "isimulated.h"
#include <QGLWidget>

class ParticleSystem;
class QDeclarativeView;
class QAudioOutput;
class QGraphicsObject;
class ParticleView;
class Game;
class MenuBackground;

#include <QDebug>
namespace Ui {
class menuroot;
}

class RootView : public QGLWidget, public ISimulated
{
    DECLARE_CLASS(RootView, QGLWidget);

    Q_OBJECT
    Q_PROPERTY(bool AAEnabled READ getAAEnabled WRITE setAAEnabled NOTIFY AAEnabledChanged)
    Q_PROPERTY(bool ParallelRenderingEnabled READ getParallelRenderingEnabled
               WRITE setParallelRenderingEnabled NOTIFY ParallelRenderingEnabledChanged)
    Q_PROPERTY(bool FPSEnabled READ getFPSEnabled WRITE setFPSEnabled NOTIFY FPSEnabledChanged)
    Q_PROPERTY(bool MusicEnabled READ getMusicEnabled WRITE setMusicEnabled NOTIFY MusicEnabledChanged)
    Q_PROPERTY(QString LevelName READ getLevelName WRITE setLevelName NOTIFY LevelNameChanged)

public:
    explicit RootView(QWidget *parent = 0);
    ~RootView();

    virtual void resizeEvent(QResizeEvent *);

    virtual void OnSimulate(float frametime);

    bool getAAEnabled(){ return AAEnabled; }
    void setAAEnabled(bool enabled);
    bool getParallelRenderingEnabled(){ return ParallelRenderingEnabled; }
    void setParallelRenderingEnabled(bool enabled);
    bool getFPSEnabled(){ return FPSEnabled; }
    void setFPSEnabled(bool enabled);
    bool getMusicEnabled(){ return MusicEnabled; }
    void setMusicEnabled(bool enabled);
    QString getLevelName(){ return LevelName; }
    void setLevelName(QString name);

    virtual void paintEvent(QPaintEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void onButtonHover(QGraphicsObject *caller, bool containsMouse);
    void onButtonClick();
    void onStartGame(QString mapname, bool newGame);
    void onSetResolution(QString key);
    void onMenuFadedOut();
    void onShowBackground();
    void onLevelIntro();
    void onGameOver();

signals:
    void AAEnabledChanged(bool);
    void ParallelRenderingEnabledChanged(bool);
    void FPSEnabledChanged(bool);
    void MusicEnabledChanged(bool);
    void LevelNameChanged(QString);
    void SelectedResolutionChanged(QString);

    void prepareIngameMenu();
    void prepareGameoverMenu();
    
private:
    enum MenuMode_e
    {
        MENU_Main = 0,
        MENU_Ingame,
        MENU_Gameover, // highscore submission
        MENU_Score
    };

    void ShowBackground();
    void HideBackground();

    void ShowMenu(MenuMode_e mode = MENU_Main);
    void HideMenu();
    void ShowGameOver();
    void ShowScore();

    void CreateGame(const char *mapname, bool newGame);
    void DestroyGame();

    void EscapePressed();

    MenuBackground *background;
    ParticleView *particleView;
    QDeclarativeView *mainQml;
    QDeclarativeView *hudQml;

    Game *game;

    bool AAEnabled;
    bool ParallelRenderingEnabled;
    bool FPSEnabled;
    bool MusicEnabled;
    QString LevelName;
};

#endif // MENUROOT_H
