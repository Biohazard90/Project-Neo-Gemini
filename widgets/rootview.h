#ifndef MENUROOT_H
#define MENUROOT_H

#include "macros.h"
#include "isimulated.h"
#include <QWidget>

#include <QtDeclarative/QDeclarativeView>
#include <QDebug>

class ParticleSystem;
class QDeclarativeView;
class QAudioOutput;
class QGraphicsObject;
class ParticleView;
class Game;
class MenuBackground;

class RootView : public QWidget, public ISimulated
{
    DECLARE_CLASS(RootView, QWidget);

    Q_OBJECT
    Q_PROPERTY(bool AAEnabled READ getAAEnabled WRITE setAAEnabled NOTIFY AAEnabledChanged)
    Q_PROPERTY(bool FPSEnabled READ getFPSEnabled WRITE setFPSEnabled NOTIFY FPSEnabledChanged)
    Q_PROPERTY(bool MusicEnabled READ getMusicEnabled WRITE setMusicEnabled NOTIFY MusicEnabledChanged)
    Q_PROPERTY(bool FullscreenEnabled READ getFullscreenEnabled WRITE setFullscreenEnabled NOTIFY FullscreenEnabledChanged)
    Q_PROPERTY(QString LevelName READ getLevelName WRITE setLevelName NOTIFY LevelNameChanged)
    Q_PROPERTY(bool GameVisible READ getGameVisible WRITE setGameVisible NOTIFY GameVisibleChanged)
    Q_PROPERTY(qreal MouseX READ getMouseX NOTIFY MouseXChanged)
    Q_PROPERTY(qreal MouseY READ getMouseY NOTIFY MouseYChanged)

public:
    explicit RootView(QWidget *parent = 0);
    ~RootView();

    static RootView *GetActiveRootView();

    virtual void resizeEvent(QResizeEvent *);

    virtual void OnSimulate(float frametime);

    bool getAAEnabled(){ return AAEnabled; }
    void setAAEnabled(bool enabled);
    bool getFPSEnabled(){ return FPSEnabled; }
    void setFPSEnabled(bool enabled);
    bool getMusicEnabled(){ return MusicEnabled; }
    void setMusicEnabled(bool enabled);
    bool getGameVisible(){ return GameVisible; }
    void setGameVisible(bool visible);
    bool getFullscreenEnabled(){ return FullscreenEnabled; }
    void setFullscreenEnabled(bool enabled);
    QString getLevelName(){ return LevelName; }
    void setLevelName(QString name);
    qreal getMouseX(){ return MouseX; }
    qreal getMouseY(){ return MouseY; }

    virtual void ExternalPaintEvent(render_context_t &context);

    Game *GetGame() const { return game; }

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
    void onAbortGame();
    void onLevelIntro();
    void onGameOver(QString nextMap);

signals:
    void AAEnabledChanged(bool);
    void FPSEnabledChanged(bool);
    void MusicEnabledChanged(bool);
    void GameVisibleChanged(bool);
    void FullscreenEnabledChanged(bool);
    void LevelNameChanged(QString);
    void SelectedResolutionChanged(QString);
    void MouseXChanged(qreal);
    void MouseYChanged(qreal);

    void prepareIngameMenu();
    void prepareGameoverMenu();
    void prepareMapTransition(QString lvl);
    
private:
    static RootView *rootViewInstance;

    enum MenuMode_e
    {
        MENU_Main = 0,
        MENU_Ingame,
        MENU_Gameover, // highscore submission
        MENU_MapTransition,
        MENU_Score
    };
    void ShowMenu(MenuMode_e mode = MENU_Main);

    void ShowBackground();
    void HideBackground();

    void HideMenu();
    void ShowGameOver();
    void ShowScore();

    void CreateGame(const char *mapname, bool newGame);
    void DestroyGame();

    void EscapePressed();

    MenuBackground *background;
    ParticleView *particleView;
    QDeclarativeView *mainQml;

    Game *game;

    bool AAEnabled;
    bool FPSEnabled;
    bool MusicEnabled;
    bool GameVisible;
    bool FullscreenEnabled;
    QString LevelName;
    qreal MouseX;
    qreal MouseY;
};

#endif // MENUROOT_H
