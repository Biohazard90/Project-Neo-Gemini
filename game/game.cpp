
#include "gamebase.h"
#include "parallelrenderer.h"

#include <QFont>


Game::Game(QObject *parent) :
    QObject(parent)
  , paused( false )
  , map( nullptr )
  , player( nullptr )
  , gametime( 0.0f )
  , gameticks( 0 )
  , gametickframe( 0.0f )
  , LiveCount( 0 )
  , livesChangedTime( 0.0f )
  , PlayerHealth( 0 )
{
    collisionManager = new CollisionManager();

    particleroot = new ParticleRoot();

    // this pointer leak but w/e
    IEventListener *thisListener = dynamic_cast<IEventListener *>(this);
    Events::GetInstance()->AddListener("player_death", thisListener);
    Events::GetInstance()->AddListener("player_health_changed", thisListener);
}

Game::~Game()
{
    // this pointer leak but w/e
    Events::GetInstance()->RemoveListener(dynamic_cast<IEventListener *>(this));

    map = nullptr;
    player = nullptr;

    DeleteAllEntities();

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
    if (map == nullptr)
    {
        context.painter->fillRect(context.x, context.y, context.w, context.h, Qt::black);
    }

    PaintGame(context);

    if (pGlobals->showfps)
    {
        PaintDebug(context);
    }
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

    if (player != nullptr)
    {
        player->OnRender(context);
    }

    particleroot->OnRender(context);

    if (map != nullptr)
    {
        map->OnRenderForeground(context);
    }
}

void Game::PaintDebug(const render_context_t &context)
{
    context.painter->setPen(Qt::red);
    context.painter->setFont(QFont("calibri", 12));

    QString fps = QString("fps: %1").arg(pGlobals->fps);
    context.painter->drawText(0, 15, fps);

    context.painter->drawText(0, 30, QString("time: %1").arg(GetGameTime()));

    //collisionManager->PaintCollisions(context);
}

void Game::LoadMap(const char *mapname)
{
    DeleteAllEntities();

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

    Input::GetInstance()->ResetButtons();

    KeyValues *event = new KeyValues("map_started");
    event->SetFloat("time", GetGameTime());
    event->SetString("mapname", mapname);
    event->SetInt("player_health", player->GetHealth());
    Events::GetInstance()->FireEvent(event);
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

    if (e != nullptr)
        SpawnEntity(e);

    return e;
}

Entity *Game::CreateEntityNoSpawn(const QString &scriptName)
{
    Entity *e = EntityFactory::GetInstance()->CreateEntityByName(scriptName);

    if (e == nullptr)
    {
        DBGWARNING("!! Unable to create entity for scriptname:" << scriptName);
        Q_ASSERT(0);
        return nullptr;
    }

    AddEntity(e);
    return e;
}

void Game::AddEntity(Entity *entity)
{
    Q_ASSERT(entity != nullptr);

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
    Q_ASSERT(entity != nullptr);

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
    Q_ASSERT(entity != nullptr && entities.contains(entity));

    entity->AddFlags(Entity::EF_KILL);
}

void Game::DeleteEntity(Entity *entity)
{
    Q_ASSERT(entity != nullptr && entities.contains(entity));

    FOREACH_QLIST(entities, Entity*, e)
    {
        if (e->GetOwner() == entity)
            e->SetOwner(nullptr);
    }
    FOREACH_QLIST_END;

    entity->OnRemove();
    if (map != nullptr)
    {
        map->OnEntityRemoved(entity);
    }
    particleroot->OnParentRemoved(entity);

    collisionManager->RemoveObject(entity);
    entities.removeOne(entity);
    delete entity;
}

void Game::DeleteAllEntities()
{
    FOREACH_QLIST(entities, Entity*, e)
    {
        DeleteEntity(e);
    }
    FOREACH_QLIST_END;
}

ParticleRoot *Game::GetParticleRoot()
{
    return particleroot;
}

Player *Game::GetPlayer()
{
    return player;
}

void Game::EndMap(const QString &nextMap)
{
    KeyValues *event = new KeyValues("map_ended");
    event->SetFloat("time", GetGameTime());

    if (player != nullptr)
    {
        event->SetInt("player_health", player->GetHealth());
    }

    Events::GetInstance()->FireEvent(event);

    emit GameEnded(nextMap);
}

void Game::ShowWarningText()
{
    emit warningText();
}

void Game::ShowCutscene(QString portraitLeft, QString portraitRight, QString titel, QString message)
{
    PortraitLeft = portraitLeft;
    PortraitRight = portraitRight;
    Titel = titel;
    Message = message;

    emit cutscene();
}

void Game::OnEvent(const char *name, KeyValues *data)
{
    if (_streq(name, "player_death"))
    {
        if (getLiveCount() > 0)
        {
            setLiveCount(getLiveCount() - 1);
            livesChangedTime = GetGameTime();
        }
    }
    else if(_streq(name, "player_health_changed"))
    {
        setPlayerHealth(data->GetInt("health"));
    }
}

void Game::setLiveCount(int count)
{
    LiveCount = count;
    emit liveCountChanged(count);
}

void Game::setPlayerHealth(int health)
{
    PlayerHealth = health;
    emit playerHealthChanged(health);
}
