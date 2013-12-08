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
#include <QtNetwork>
#include <QTimer>


Statistics Statistics::instance;

Statistics::Statistics()
    : tempGameStartedTime(-1.0f)
    , networkManager(nullptr)
    , fileUploadEnabled(true)
    , uploadIsBlocking(false)
    , filterGameTime(0.0f)
    , filterGameWon(false)
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

    SetGraphFilterGameTime(20.0f);
    //SetGraphFilterGameWon(true);
    SetGraphFilterLevel("level_1");

    IEventListener *listener = dynamic_cast<IEventListener *>(this);
    Events::GetInstance()->AddListener("player_death", listener);
    Events::GetInstance()->AddListener("player_health_changed", listener);
    Events::GetInstance()->AddListener("map_started", listener);
    Events::GetInstance()->AddListener("map_ended", listener);
    Events::GetInstance()->AddListener("map_aborted", listener);
    Events::GetInstance()->AddListener("object_destroyed", listener);
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

        const float time = game->GetGameTime();

        for (auto &damages : currentSet.playerGames.last().playerDamages)
        {
            if (damages.time == time)
            {
                return;
            }
        }

        Player *player = game->GetPlayer();

        Q_ASSERT(player != nullptr);

        if (player != nullptr)
        {
            StatPlayerDamage stat;
            stat.time = time;
            stat.death = !player->IsAlive();
            stat.inflictorName = data->GetString("inflictor_name", "unknown");
            stat.inflictorClass = data->GetString("inflictor_data");
            stat.weaponName = data->GetString("weapon_name");
            stat.weaponClass = data->GetString("weapon_data");
            stat.normalizedPosition = Vector2D(data->GetFloat("x"), data->GetFloat("y"));

            Camera::GetInstance()->ToScreen(stat.normalizedPosition);
            stat.normalizedPosition.x /= pGlobals->screen_width;
            stat.normalizedPosition.y /= pGlobals->screen_height;

            currentSet.playerGames.last().playerDamages.append(stat);
        }
    }
    else if (_streq("object_destroyed", name))
    {
        StatObjectDestroyed stat;
        stat.time = game->GetGameTime();
        stat.objectName = data->GetString("object_name");
        stat.objectClass = data->GetString("object_class");
        stat.score = data->GetInt("score");

        stat.normalizedPosition = Vector2D(data->GetFloat("x"), data->GetFloat("y"));

        Camera::GetInstance()->ToScreen(stat.normalizedPosition);
        stat.normalizedPosition.x /= pGlobals->screen_width;
        stat.normalizedPosition.y /= pGlobals->screen_height;

        currentSet.playerGames.last().destroyedObjects.append(stat);
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
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO8859-1\""));
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
            if (damage.inflictorClass.length() > 0)
                XMLWriteString(doc, damageEntry, "inflictorclass", damage.inflictorClass);
            if (damage.weaponName.length() > 0)
                XMLWriteString(doc, damageEntry, "weaponname", damage.weaponName);
            if (damage.weaponClass.length() > 0)
                XMLWriteString(doc, damageEntry, "weaponclass", damage.weaponClass);
            XMLWriteFloat(doc, damageEntry, "time", damage.time);
            XMLWriteFloat(doc, damageEntry, "screenx", damage.normalizedPosition.x);
            XMLWriteFloat(doc, damageEntry, "screeny", damage.normalizedPosition.y);
            XMLWriteInt(doc, damageEntry, "death", damage.death ? 1 : 0);
        }

        QDomElement objects = doc.createElement("destroyedobjects");
        gameEntry.appendChild(objects);

        for (auto &object : game.destroyedObjects)
        {
            QDomElement objectEntry = doc.createElement("object");
            objects.appendChild(objectEntry);

            XMLWriteString(doc, objectEntry, "objectname", object.objectName);
            if (object.objectClass.length() > 0)
                XMLWriteString(doc, objectEntry, "objectclass", object.objectClass);
            XMLWriteInt(doc, objectEntry, "score", object.score);
            XMLWriteFloat(doc, objectEntry, "time", object.time);
            XMLWriteFloat(doc, objectEntry, "screenx", object.normalizedPosition.x);
            XMLWriteFloat(doc, objectEntry, "screeny", object.normalizedPosition.y);
        }
    }

    QString filename = OSLocalPath(relativePath);

    QString str;
    QTextStream tmp(&str);

    doc.save(tmp, 4);

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    if (file.isOpen())
    {
        file.write(str.toLocal8Bit());
        file.close();
    }
    else
    {
        qWarning() << "!! Unable to write stats: " << filename;
    }

    if (fileUploadEnabled)
    {
        UploadData(filename, str.toLocal8Bit());
    }
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

    FOREACH_QDOM_CHILD(games, game, gameNode)
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

        FOREACH_QDOM_CHILD(damages, damage, damageNode)
        {
            StatPlayerDamage damage;

            damage.inflictorName = XMLReadString(damageNode, "inflictorname");
            damage.inflictorClass = XMLReadString(damageNode, "inflictorclass");
            damage.weaponName = XMLReadString(damageNode, "weaponname");
            damage.weaponClass = XMLReadString(damageNode, "weaponclass");
            damage.time = XMLReadFloat(damageNode, "time");
            damage.normalizedPosition.x = XMLReadFloat(damageNode, "screenx");
            damage.normalizedPosition.y = XMLReadFloat(damageNode, "screeny");
            damage.death = XMLReadInt(damageNode, "death") != 0;

            game.playerDamages.append(damage);
        }
        FOREACH_QDOM_CHILD_END;

        QDomElement objects = gameNode.firstChildElement("destroyedobjects");

        if (objects.isNull())
        {
            DBGWARNING("Stats file without destroyed objects: " << filename);
            return false;
        }

        FOREACH_QDOM_CHILD(objects, object, objectNode)
        {
            StatObjectDestroyed object;

            object.objectName = XMLReadString(objectNode, "objectname");
            object.objectClass = XMLReadString(objectNode, "objectclass");
            object.score = XMLReadInt(objectNode, "score");
            object.time = XMLReadFloat(objectNode, "time");
            object.normalizedPosition.x = XMLReadFloat(objectNode, "screenx");
            object.normalizedPosition.y = XMLReadFloat(objectNode, "screeny");

            game.destroyedObjects.append(object);
        }
        FOREACH_QDOM_CHILD_END;

        set.playerGames.append(game);
    }
    FOREACH_QDOM_CHILD_END;

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
    GenerateScoreDistribution();
    GenerateParticipation();
    GenerateEnemyEffectiveness();
}

void Statistics::SetGraphFilterGameTime(float time)
{
    filterGameTime = time;
}

void Statistics::SetGraphFilterGameWon(bool onlyGamesWon)
{
    filterGameWon = onlyGamesWon;
}

void Statistics::SetGraphFilterLevel(const QString &level)
{
    filterLevel = level;
}

void Statistics::SortGames(QHash<QString, QList<StatGame *>> &registeredGames)
{
    registeredGames.clear();

    for (auto &set : sets)
    {
        for (auto &game : set.playerGames)
        {
            if (FilterGame(game))
            {
                continue;
            }

            QString hash(game.mapname + game.maphash);

            if (!registeredGames.contains(hash))
            {
                registeredGames.insert(hash, QList<StatGame *>());
            }

            registeredGames[hash].append(&game);
        }
    }
}

void Statistics::SortGames(QHash<QString, QList<QPair<StatSet *, StatGame *>>> &registeredGames)
{
    registeredGames.clear();

    for (auto &set : sets)
    {
        for (auto &game : set.playerGames)
        {
            if (FilterGame(game))
            {
                continue;
            }

            QString hash(game.mapname + game.maphash);

            if (!registeredGames.contains(hash))
            {
                registeredGames.insert(hash, QList<QPair<StatSet *, StatGame *>>());
            }

            QPair<StatSet *, StatGame*> pair;
            pair.first = &set;
            pair.second = &game;
            registeredGames[hash].append(pair);
        }
    }
}

bool Statistics::FilterGame(const StatGame &game)
{
    if (filterGameTime > 0.0f
            && game.duration < filterGameTime)
    {
        return true;
    }

    if (filterGameWon
            && !game.DidPlayerWin())
    {
        return true;
    }

    if (filterLevel.length() > 0
            && filterLevel != game.mapname)
    {
        return true;
    }

    return false;
}

QString Statistics::GetFilterString()
{
    QStringList list;

    if (filterGameTime > 0.0f)
    {
        list.append(QString("min play time: %1s").arg(filterGameTime));
    }

    if (filterGameWon)
    {
        list.append("game won");
    }

    QString str;

    for (int i = 0; i < list.length(); i++)
    {
        str += list[i];

        if (i < list.length() - 1)
        {
            str += ", ";
        }
    }

    if (!list.empty())
    {
        str = "Filters: " + str;
    }

    return str;
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
            plotter.PaintFilterString(GetFilterString());
            plotter.PlotTimeLine(0.0f, maxTime + 10.0f, deathTimes);
            plotter.SaveTo("deathtimes_" + list.first()->GetFileSuffix());
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
            plotter.PaintFilterString(GetFilterString());
            plotter.PlotTimeLine(0.0f, maxTime + 10.0f, damageTimes);
            plotter.SaveTo("damagetimes_" + list.first()->GetFileSuffix());
        }
    }
}

void Statistics::GenerateScoreDistribution()
{
    QHash<QString, QList<StatGame *>> registeredGames;
    SortGames(registeredGames);

    for (auto &list : registeredGames)
    {
        QVector<float> score;
        QVector<QString> labels;

        for (auto *game : list)
        {
            if (game->destroyedObjects.empty())
            {
                continue;
            }

            for (auto &objectEvent : game->destroyedObjects)
            {
                QString tag = objectEvent.objectName + " (" + objectEvent.objectClass + ")";

                int index = labels.indexOf(tag);

                if (index < 0)
                {
                    labels.append(tag);
                    score.append(objectEvent.score);
                }
                else
                {
                    score[index] += objectEvent.score;
                }
            }
        }

        if (score.length() > 0)
        {
            QString title("Score distribution: ");
            title += list.first()->mapname;

            Plotter plotter(title, 786, 800);
            plotter.PaintFilterString(GetFilterString());
            plotter.PlotPieChart(score, labels);
            plotter.SaveTo("scoredist_" + list.first()->GetFileSuffix());
        }
    }
}

void Statistics::GenerateParticipation()
{
    QHash<QString, QList<QPair<StatSet *, StatGame *>>> registeredGames;
    SortGames(registeredGames);

    for (auto &list : registeredGames)
    {
        QVector<float> gameCount;
        QVector<QString> labels;
        QVector<float> gameWonCount;

        for (auto &pair : list)
        {
            int index = labels.indexOf(pair.first->username);

            if (index < 0)
            {
                labels.append(pair.first->username);
                gameCount.append(1.0f);
                gameWonCount.append(pair.second->DidPlayerWin() ? 1.0f : 0.0f);
            }
            else
            {
                gameCount[index] += 1.0f;

                if (pair.second->DidPlayerWin())
                    gameWonCount[index] += 1.0f;
            }
        }

        if (gameCount.length() > 0)
        {
            QVector<float> winRatios;
            QVector<QString> tags;

            for (int i = 0; i < gameCount.length(); i++)
            {
                winRatios.append(gameWonCount[i] / gameCount[i]);

                QString tag = FormatString("%.f%% of total (%.f%% won of played)",
                                           gameCount[i] / float(list.length()) * 100.0f, winRatios.last() * 100.0f);
                tags.append(tag);
            }

            QString title("Participation: ");
            title += list.first().second->mapname;

            Plotter plotter(title, 1024, 512);
            plotter.PaintFilterString(GetFilterString());
            plotter.PlotBarChart(gameCount, labels, &winRatios, &tags);
            plotter.SaveTo("participation_" + list.first().second->GetFileSuffix());
        }
    }
}

void Statistics::GenerateEnemyEffectiveness()
{
    QVector<QString> labels;
    QVector<float> destructionCount;
}

void Statistics::UploadData(const QString &filename, const QByteArray &statsData)
{
//    QFile file(filename);
//    file.open(QFile::ReadOnly);

//    if (!file.isOpen())
//    {
//        return;
//    }

    //qDebug() << filename;
    QByteArray postData;

    QString bound="margin";
    QByteArray data(QString("--" + bound + "\r\n").toLocal8Bit());

    data.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
    data.append("upload.php\r\n");
    data.append(QString("--" + bound + "\r\n").toLocal8Bit());
    data.append("Content-Disposition: form-data; name=\"uploaded\"; filename=\"");
    data.append(filename.toLocal8Bit());
    data.append("\"\r\n");
    data.append("Content-Type: text/xml\r\n\r\n");

    data.append(statsData);
    data.append("\r\n");
    data.append("--" + bound + "--\r\n");
    postData = data;

    //file.close();

    QUrl resultsURL = QUrl("http://bio2k.homepage.t-online.de/hig_game_design/upload.php");

    if (networkManager == nullptr)
    {
        networkManager = new QNetworkAccessManager(qApp);
        QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyReceived(QNetworkReply*)));
    }

    QNetworkRequest request(resultsURL);
    request.setRawHeader(QString("Content-Type").toLocal8Bit(),QString("multipart/form-data; boundary=" + bound).toLocal8Bit());
    request.setRawHeader(QString("Content-Length").toLocal8Bit(), QString::number(postData.length()).toLocal8Bit());

    QNetworkReply *reply = networkManager->post(request, postData);

    if (uploadIsBlocking)
    {
        QEventLoop loop;
        QTimer timeoutTimer;

        QObject::connect(&timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timeoutTimer.start(500);

        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

void Statistics::onReplyReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "!! Statistics network error: " << reply->errorString();
        //qDebug() << reply->readAll();
    }
}
