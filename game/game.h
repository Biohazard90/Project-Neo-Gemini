#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>


#include "isimulated.h"
#include "irenderable.h"
#include "igamecontext.h"
#include "events.h"

class ParticleRoot;
struct render_context_t;
class Map;
class Entity;
class ParallelRenderer;
class Player;
class CollisionManager;

class Game : public QObject,
        public IGameContext,
        public ISimulated,
        public IRenderable,
        public IEventListener
{
    Q_OBJECT
    Q_PROPERTY(int LiveCount READ getLiveCount WRITE setLiveCount NOTIFY liveCountChanged)
    Q_PROPERTY(int PlayerHealth READ getPlayerHealth WRITE setPlayerHealth NOTIFY playerHealthChanged)
    Q_PROPERTY(QString cutsceneMessage MEMBER Message)
    Q_PROPERTY(QString cutsceneTitel MEMBER Titel)
    Q_PROPERTY(QString cutscenePortraitLeft MEMBER PortraitLeft)
    Q_PROPERTY(QString cutscenePortraitRight MEMBER PortraitRight)

public:
    explicit Game(QObject *parent = 0);
    ~Game();

    virtual void OnSimulate(float frametime);
    virtual void OnRender(const render_context_t &context);

    void LoadMap(const char *mapname);

    bool IsPaused();
    void SetPaused(bool paused);

    virtual float GetGameTime();
    virtual Entity *CreateEntity(const QString &scriptName);
    virtual Entity *CreateEntityNoSpawn(const QString &scriptName);
    virtual void AddEntity(Entity *entity);
    virtual void SpawnEntity(Entity *entity);
    virtual void RemoveEntity(Entity *entity);

    virtual ParticleRoot *GetParticleRoot();
    virtual Player *GetPlayer();
    virtual Map *GetMap() { return map; }

    virtual void EndMap();

    virtual void ShowWarningText();
    virtual void ShowCutscene(QString portraitLeft,QString portraitRight,QString titel,QString message);

    virtual void OnEvent(const char *name, KeyValues *data);


    int getLiveCount(){ return LiveCount; }
    int getPlayerHealth(){ return PlayerHealth; }
    
signals:
    void GameEnded();
    void warningText();
    void cutscene();
    void liveCountChanged(int);
    void playerHealthChanged(int);
    
public slots:
    void setLiveCount(int count);
    void setPlayerHealth(int health);

private:
    void PaintGame(const render_context_t &context);
    void PaintDebug(const render_context_t &context);

    void UpdateGameTime(float frametime);
    void DeleteEntity(Entity *entity);
    void DeleteAllEntities();

    QList<Entity*> entities;
    Map *map;
    Player *player;
    ParticleRoot *particleroot;

    int LiveCount;
    float livesChangedTime;
    int PlayerHealth;

    QString PortraitLeft;
    QString PortraitRight;
    QString Titel;
    QString Message;

    CollisionManager *collisionManager;

    bool paused;
    float gametime;
    int gameticks;
    float gametickframe;
};

#endif // GAME_H
