
#include "gamebase.h"
#include "parallelrenderer.h"

#include <QFont>


Game::Game(QObject *parent) :
    QObject(parent)
  , paused( false )
  , map( NULL )
  , player( NULL )
  , gametime( 0.0f )
  , gameticks( 0 )
  , gametickframe( 0.0f )
  , LiveCount( 0 )
  , livesChangedTime( 0.0f )
{
    collisionManager = new CollisionManager();

    particleroot = new ParticleRoot();
}

Game::~Game()
{
    map = NULL;
    player = NULL;

    FOREACH_QLIST(entities, Entity*, e)
    {
        DeleteEntity(e);
    }
    FOREACH_QLIST_END;

    delete particleroot;

    delete collisionManager;
}

bool Game::IsPaused()
{
    return paused;
}

void Game::SetPaused(bool paused)
{
    this->paused = paused;
}

void Game::OnSimulate(float frametime)
{
    if (!paused)
    {
        frametime *= GAME_SPEED_SCALE;

        UpdateGameTime(frametime);

        FOREACH_QLIST(entities, Entity*, e)
        {
            e->OnSimulate(frametime);
            e->OnMove(frametime);

            if (e->HasFlags(Entity::EF_KILL))
            {
                DeleteEntity(e);
            }
        }
        FOREACH_QLIST_END;

        FOREACH_QLIST(entities, Entity*, e)
        {
            if (e->HasFlags(Entity::EF_TELEPORTED))
            {
                e->RemoveFlags(Entity::EF_TELEPORTED);
                collisionManager->TeleportObject(e);
            }
        }
        FOREACH_QLIST_END;

        collisionManager->OnSimulate(frametime);

        particleroot->OnSimulate(frametime);

        if (LiveCount <= 0
                && livesChangedTime > 0.0f
                && livesChangedTime + 2.0f < GetGameTime())
        {
            livesChangedTime = 0.0f;

            EndMap();
        }
    }
}

void Game::OnRender(const render_context_t &context)
{
    PaintGame(context);

    if (pGlobals->showfps)
        PaintDebug(context);
}

void Game::PaintGame(const render_context_t &context)
{
    FOREACH_QLIST(entities, Entity*, e)
    {
        if (!e->IsPlayer())
        {
            e->OnRender(context);
        }
    }
    FOREACH_QLIST_END;

    if (player != NULL)
    {
        player->OnRender(context);
    }

    particleroot->OnRender(context);

    if (map != NULL)
    {
        map->OnRenderForeground(context);
    }
}

void Game::PaintDebug(const render_context_t &context)
{
    context.painter->setPen(Qt::red);
    context.painter->setFont(QFont("calibri", 12));

    {
        QString fps = QString("fps: %1").arg((pGlobals->frametime > 0) ? (1.0f / pGlobals->frametime) : 0);
        context.painter->drawText(0, 15, fps);
    }

    context.painter->drawText(0, 30, QString("time: %1").arg(GetGameTime()));

    collisionManager->PaintCollisions(context);
}

void Game::LoadMap(const char *mapname)
{
    DESTROY_QLIST(entities);

    setLiveCount(PLAYER_DEFAULT_LIVE_COUNT);

    Camera::GetInstance()->SetTranslation(
                Vector2D(0, -NORMAL_SCREEN_HEIGHT / 2));

    std::string str(PATH_MAP_ROOT);
    str += mapname;
    str += ".xml";

    map = Map::CreateMapFromXML(str.c_str());
    AddEntity(map);
    SpawnEntity(map);

    player = (Player*)CreateEntity("player");
}

void Game::UpdateGameTime(float frametime)
{
    const float tickrate = 1.0f / 33.0f;
    gametickframe += frametime;

    if (gametickframe > tickrate)
    {
        gameticks += floor(gametickframe / tickrate);
        gametickframe = fmod(gametickframe, tickrate);
    }

    gametime = gameticks * tickrate + gametickframe;
}

float Game::GetGameTime()
{
    return gametime;
}

Entity *Game::CreateEntity(const QString &scriptName)
{
    Entity *e = CreateEntityNoSpawn(scriptName);

    if (e != NULL)
        SpawnEntity(e);

    return e;
}

Entity *Game::CreateEntityNoSpawn(const QString &scriptName)
{
    Entity *e = EntityFactory::GetInstance()->CreateEntityByName(scriptName);

    if (e == NULL)
    {
        DBGWARNING("!! Unable to create entity for scriptname:" << scriptName);
        Q_ASSERT(0);
        return NULL;
    }

    AddEntity(e);
    return e;
}

void Game::AddEntity(Entity *entity)
{
    Q_ASSERT(entity != NULL);

    if (entities.contains(entity))
    {
        DBGWARNING("!! Entity already added");
        Q_ASSERT(0);
        return;
    }

    entity->SetGameContext(this);
    entities.push_back(entity);

    collisionManager->AddObject(entity);
}

void Game::SpawnEntity(Entity *entity)
{
    Q_ASSERT(entity != NULL);

    if (!entities.contains(entity))
    {
        DBGWARNING("!! Entity can't be spawned without being added to the game");
        Q_ASSERT(0);
        return;
    }

    entity->Spawn();
    collisionManager->TeleportObject(entity);
}

void Game::RemoveEntity(Entity *entity)
{
    Q_ASSERT(entity != NULL && entities.contains(entity));

    entity->AddFlags(Entity::EF_KILL);
}

void Game::DeleteEntity(Entity *entity)
{
    Q_ASSERT(entity != NULL && entities.contains(entity));

    FOREACH_QLIST(entities, Entity*, e)
    {
        if (e->GetOwner() == entity)
            e->SetOwner(NULL);
    }
    FOREACH_QLIST_END;

    entity->OnRemove();
    if (map != NULL)
    {
        map->OnEntityRemoved(entity);
    }
    particleroot->OnParentRemoved(entity);

    collisionManager->RemoveObject(entity);
    entities.removeOne(entity);
    delete entity;
}

ParticleRoot *Game::GetParticleRoot()
{
    return particleroot;
}

Player *Game::GetPlayer()
{
    return player;
}

void Game::PlayerDied()
{
    if (getLiveCount() > 0)
    {
        setLiveCount(getLiveCount() - 1);
        livesChangedTime = GetGameTime();
    }
}

void Game::EndMap()
{
    emit GameEnded();
}

void Game::setLiveCount(int count)
{
    LiveCount = count;
    emit liveCountChanged(count);
}
