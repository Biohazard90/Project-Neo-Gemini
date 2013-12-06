#include "../base.h"
#include "statistics.h"
#include "rootview.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "plotter.h"

#include <QtXml>
#include <QTextStream>
#include <QFile>



Statistics Statistics::instance;

Statistics::Statistics()
    : tempGameStartedTime(-1.0f)
{
}

Statistics *Statistics::GetInstance()
{
    return &instance;
}

void Statistics::Init()
{
    currentSet.sessionDuration = 0.0f;
    currentSet.username = OSUserName();
    currentSet.date = QDateTime::currentDateTime();

    PushGame();

    IEventListener *listener = dynamic_cast<IEventListener *>(this);
    Events::GetInstance()->AddListener("player_death", listener);
    Events::GetInstance()->AddListener("player_health_changed", listener);
    Events::GetInstance()->AddListener("map_started", listener);
    Events::GetInstance()->AddListener("map_ended", listener);
    Events::GetInstance()->AddListener("map_aborted", listener);
}

void Statistics::Shutdown()
{
    Events::GetInstance()->RemoveListener(dynamic_cast<IEventListener *>(this));
}

void Statistics::OnEvent(const char *name, KeyValues *data)
{
    RootView *rootView = RootView::GetActiveRootView();
    Game *game = rootView ? rootView->GetGame() : nullptr;

    if (game == nullptr)
    {
        Q_ASSERT(0);
        return;
    }

    if (_streq("player_death", name)
            || _streq("player_health_changed", name))
    {
        if (!data->GetBool("decreasing", true))
        {
            return;
        }

        Player *player = game->GetPlayer();

        Q_ASSERT(player != nullptr);

        if (player != nullptr)
        {
            StatPlayerDamage stat;
            stat.time = game->GetGameTime();
            stat.death = !player->IsAlive();
            stat.inflictorName = data->GetString("inflictor_name", "unknown");
            stat.inflictorClass = data->GetString("inflictor_data");
            stat.normalizedPosition = Vector2D(data->GetFloat("x"), data->GetFloat("y"));

            Camera::GetInstance()->ToScreen(stat.normalizedPosition);
            stat.normalizedPosition.x /= pGlobals->screen_width;
            stat.normalizedPosition.y /= pGlobals->screen_height;

            currentSet.playerGames.last().playerDamages.append(stat);
        }
    }
    else if (_streq("map_started", name))
    {
        QString filename("content/maps/");
        filename += data->GetString("mapname");
        filename += ".xml";

        currentSet.playerGames.last().startHealth = data->GetInt("player_health");
        currentSet.playerGames.last().mapname = data->GetString("mapname");
        currentSet.playerGames.last().maphash = GetFileHash(filename);

        tempGameStartedTime = data->GetFloat("time");
    }
    else if (_streq("map_ended", name)
             || _streq("map_aborted", name))
    {
        currentSet.playerGames.last().endHealth = data->GetInt("player_health");

        if (tempGameStartedTime >= 0.0f)
        {
            currentSet.playerGames.last().duration = data->GetFloat("time") - tempGameStartedTime;
        }
        else
        {
            currentSet.playerGames.last().duration = 0.0f;
        }

        currentSet.playerGames.last().aborted = _streq("map_aborted", name);
        tempGameStartedTime = -1.0f;

        PushGame();

        SaveSet(currentSet);
    }
    else
    {
        DBGWARNING("Unhandled statistics event: " << name);
    }
}

void Statistics::PushGame()
{
    currentSet.playerGames.append(StatGame());
}

void Statistics::SaveSet(const StatSet &set)
{
    QString relativePath = OSLocalPath("stats");

    CreateDirIfNotExists(relativePath);

    relativePath += "/" + set.username + " " + set.date.toString(Qt::ISODate) + ".xml";
    relativePath = relativePath.replace(':', '_');
    relativePath = relativePath.replace(' ', '_');

    QDomDocument doc;
    QDomElement root = doc.createElement("stats");
    doc.appendChild(root);

    XMLWriteString(doc, root, "username", set.username);
    XMLWriteFloat(doc, root, "sessionduration", pGlobals->curtime);
    XMLWriteString(doc, root, "date", set.date.toString(Qt::ISODate));

    QDomElement games = doc.createElement("games");
    root.appendChild(games);

    for (auto &game : set.playerGames)
    {
        if (game.mapname.length() < 1)
        {
            continue;
        }

        QDomElement gameEntry = doc.createElement("game");
        games.appendChild(gameEntry);

        XMLWriteString(doc, gameEntry, "mapname", game.mapname);
        XMLWriteString(doc, gameEntry, "maphash", game.maphash);
        XMLWriteInt(doc, gameEntry, "starthealth", game.startHealth);
        XMLWriteInt(doc, gameEntry, "endhealth", game.endHealth);
        XMLWriteFloat(doc, gameEntry, "duration", game.duration);
        XMLWriteInt(doc, gameEntry, "aborted", game.aborted ? 1 : 0);

        QDomElement damages = doc.createElement("damages");
        gameEntry.appendChild(damages);

        for (auto &damage : game.playerDamages)
        {
            QDomElement damageEntry = doc.createElement("damage");
            damages.appendChild(damageEntry);

            XMLWriteString(doc, damageEntry, "inflictorname", damage.inflictorName);
            XMLWriteString(doc, damageEntry, "inflictorclass", damage.inflictorClass);
            XMLWriteFloat(doc, damageEntry, "time", damage.time);
            XMLWriteFloat(doc, damageEntry, "screenx", damage.normalizedPosition.x);
            XMLWriteFloat(doc, damageEntry, "screeny", damage.normalizedPosition.y);
            XMLWriteInt(doc, damageEntry, "death", damage.death ? 1 : 0);
        }
    }

    QFile file(OSLocalPath(relativePath));
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    doc.save(out, 4);

    file.close();
}

bool Statistics::LoadSet(const QString &filename, StatSet &set)
{
    QDomElement root;

    if (!OpenXMLFile(filename, root))
    {
        DBGWARNING("failed to open stats: " << filename);
        return false;
    }

    set.username = XMLReadString(root, "username");
    set.date = QDateTime::fromString(XMLReadString(root, "date"), Qt::ISODate);
    set.sessionDuration = XMLReadFloat(root, "sessionduration");

    QDomElement games = root.firstChildElement("games");

    if (games.isNull())
    {
        DBGWARNING("Stats file without games: " << filename);
        return false;
    }

    for (QDomElement gameNode = games.firstChildElement();
         !gameNode.isNull();
         gameNode = gameNode.nextSiblingElement())
    {
        StatGame game;

        game.mapname = XMLReadString(gameNode, "mapname");
        game.maphash = XMLReadString(gameNode, "maphash");
        game.startHealth = XMLReadInt(gameNode, "starthealth");
        game.endHealth = XMLReadInt(gameNode, "endhealth");
        game.duration = XMLReadFloat(gameNode, "duration");
        game.aborted = XMLReadInt(gameNode, "aborted") != 0;

        QDomElement damages = gameNode.firstChildElement("damages");

        if (damages.isNull())
        {
            DBGWARNING("Stats file without damages: " << filename);
            return false;
        }

        for (QDomElement damageNode = damages.firstChildElement();
             !damageNode.isNull();
             damageNode = damageNode.nextSiblingElement())
        {
            StatPlayerDamage damage;

            damage.inflictorName = XMLReadString(damageNode, "inflictorname");
            damage.inflictorClass = XMLReadString(damageNode, "inflictorclass");
            damage.time = XMLReadFloat(damageNode, "time");
            damage.normalizedPosition.x = XMLReadFloat(damageNode, "screenx");
            damage.normalizedPosition.y = XMLReadFloat(damageNode, "screeny");
            damage.death = XMLReadInt(damageNode, "death") != 0;

            game.playerDamages.append(damage);
        }

        set.playerGames.append(game);
    }

    return true;
}

void Statistics::LoadAllStats()
{
    sets.clear();

    QDirIterator itr(OSLocalPath("stats/"));
    while (itr.hasNext())
    {
        QString nextPath = itr.next();

        if (nextPath.endsWith("."))
        {
            continue;
        }

        if (nextPath.endsWith(".xml"))
        {
            StatSet set;

            if (LoadSet(nextPath, set))
            {
                sets.append(set);
            }
        }
    }
}

void Statistics::GenerateGraphs()
{
    LoadAllStats();

    CreateDirIfNotExists(OSLocalPath(PATH_GRAPHS_ROOT));

    GenerateDeathTimelines();
    GenerateDamageTimelines();
}

void Statistics::SortGames(QHash<QString, QList<StatGame *>> &registeredGames)
{
    registeredGames.clear();

    for (auto &set : sets)
    {
        for (auto &game : set.playerGames)
        {
            QString hash(game.mapname + game.maphash);

            if (!registeredGames.contains(hash))
            {
                registeredGames.insert(hash, QList<StatGame *>());
            }

            registeredGames[hash].append(&game);
        }
    }
}

void Statistics::GenerateDeathTimelines()
{
    QHash<QString, QList<StatGame *>> registeredGames;
    SortGames(registeredGames);

    for (auto &list : registeredGames)
    {
        QVector<float> deathTimes;
        float maxTime = 0.0f;

        for (auto *game : list)
        {
            if (game->playerDamages.empty())
            {
                continue;
            }

            if (!game->playerDamages.last().death)
            {
                continue;
            }

            deathTimes.append(game->playerDamages.last().time);

            maxTime = qMax(maxTime, deathTimes.last());
        }

        if (!deathTimes.empty())
        {
            QString title("Death times: ");
            title += list.first()->mapname;

            Plotter plotter(title, 1024, 196);
            plotter.PlotTimeLine(0.0f, maxTime + 10.0f, deathTimes);
            plotter.SaveTo("deathtimes_" + list.first()->mapname + "_" + list.first()->mapname);
        }
    }
}

void Statistics::GenerateDamageTimelines()
{
    QHash<QString, QList<StatGame *>> registeredGames;
    SortGames(registeredGames);

    for (auto &list : registeredGames)
    {
        QVector<float> damageTimes;
        float maxTime = 0.0f;

        for (auto *game : list)
        {
            if (game->playerDamages.empty())
            {
                continue;
            }

            for (auto &damageEvent : game->playerDamages)
            {
                damageTimes.append(damageEvent.time);
            }

            maxTime = qMax(maxTime, damageTimes.last());
        }

        if (!damageTimes.empty())
        {
            QString title("Damage times: ");
            title += list.first()->mapname;

            Plotter plotter(title, 1024, 196);
            plotter.PlotTimeLine(0.0f, maxTime + 10.0f, damageTimes);
            plotter.SaveTo("damagetimes_" + list.first()->mapname + "_" + list.first()->mapname);
        }
    }
}
