#ifndef STATISTICS_H
#define STATISTICS_H

#include "events.h"

#include <QDateTime>

class QNetworkAccessManager;
struct StatSet;

struct StatPlayerDamage
{
    QString inflictorName;
    QString inflictorClass;
    QString weaponName;
    QString weaponClass;

    float time;
    Vector2D normalizedPosition;
    bool death;
};

struct StatObjectDestroyed
{
    QString objectName;
    QString objectClass;

    int score;
    float time;
    Vector2D normalizedPosition;
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
    QList<StatObjectDestroyed> destroyedObjects;

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

class Statistics : public QObject, public IEventListener
{
    Q_OBJECT

    Statistics();
    static Statistics instance;

public:

    static Statistics *GetInstance();

    void Init();
    void Shutdown();

    void SetUploadBlocking(bool isBlocking) { uploadIsBlocking = isBlocking; }

    virtual void OnEvent(const char *name, KeyValues *data);

    void GenerateGraphs();

public slots:


private:

    void PushGame();
    void SaveSet(const StatSet &set);

    void LoadAllStats();
    bool LoadSet(const QString &filename, StatSet &set);

    void SortGames(QHash<QString, QList<StatGame *>> &registeredGames);
    void SortGames(QHash<QString, QList<QPair<StatSet *, StatGame *>>> &registeredGames);

    void GenerateDeathTimelines();
    void GenerateDamageTimelines();
    void GenerateScoreDistribution();
    void GenerateParticipation();

    void UploadFile(const QString &filename);

    float tempGameStartedTime;

    QList<StatSet> sets;

    bool fileUploadEnabled;
    bool uploadIsBlocking;
    QNetworkAccessManager *networkManager;

    StatSet currentSet;
};

#endif // STATISTICS_H
