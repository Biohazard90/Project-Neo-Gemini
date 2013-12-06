#ifndef STATISTICS_H
#define STATISTICS_H

#include "events.h"

#include <QDateTime>

struct StatPlayerDamage
{
    QString inflictorName;
    QString inflictorClass;

    float time;
    Vector2D normalizedPosition;
    bool death;
};

struct StatGame
{
    QString mapname;
    QString maphash;
    float duration;

    int startHealth;
    int endHealth;

    bool aborted;

    QList<StatPlayerDamage> playerDamages;

    QString GetFileSuffix()
    {
        return mapname + "_" + maphash;
    }
};

struct StatSet
{
    QString username;
    float sessionDuration;
    QDateTime date;

    QList<StatGame> playerGames;
};

class Statistics : public IEventListener
{
    Statistics();
    static Statistics instance;

public:

    static Statistics *GetInstance();

    void Init();
    void Shutdown();

    virtual void OnEvent(const char *name, KeyValues *data);

    void GenerateGraphs();

private:

    void PushGame();
    void SaveSet(const StatSet &set);

    void LoadAllStats();
    bool LoadSet(const QString &filename, StatSet &set);

    void SortGames(QHash<QString, QList<StatGame *>> &registeredGames);

    void GenerateDeathTimelines();
    void GenerateDamageTimelines();

    float tempGameStartedTime;

    QList<StatSet> sets;

    StatSet currentSet;
};

#endif // STATISTICS_H
