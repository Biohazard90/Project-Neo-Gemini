#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>


#include "isimulated.h"
#include "irenderable.h"
#include "igamecontext.h"

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
        public IRenderable
{
    Q_OBJECT
    Q_PROPERTY(int LiveCount READ getLiveCount WRITE setLiveCount NOTIFY liveCountChanged)

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

    virtual void PlayerDied();
    virtual void PlayerHealthChanged(int health);
    virtual void EndMap();

    int getLiveCount(){ return LiveCount; }
    
signals:
    void GameEnded();
    void liveCountChanged(int);
    
public slots:
    void setLiveCount(int count);

private:
    void PaintGame(const render_context_t &context);
    void PaintDebug(const render_context_t &context);

    void UpdateGameTime(float frametime);
    void DeleteEntity(Entity *entity);

    QList<Entity*> entities;
    Map *map;
    Player *player;
    ParticleRoot *particleroot;

    int LiveCount;
    float livesChangedTime;

    CollisionManager *collisionManager;

    bool paused;
    float gametime;
    int gameticks;
    float gametickframe;
};

#endif // GAME_H
