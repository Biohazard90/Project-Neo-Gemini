#ifndef RANKING_H
#define RANKING_H

#include <QObject>
#include <QList>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class Ranking : public QObject
{
    Q_OBJECT

public:
    static Ranking *GetInstance();

    void Update();
    void Reset();

    void RequestHighscores();
    void PostHighscore(const QString &name, int score);

signals:
    void updateHighscore();

public slots:

    void onSubmit();
    int getScore(int i);
    QString getPlayer(int i);

    void onScoreReceived(QNetworkReply *reply);

private:
    Ranking();

    void SortRanking();

    void CreateNetworkAccessManagerOnDemand();

    static Ranking instance;
    QString path;

    struct score_t
    {
        QString name;
        int score;
    };

    QList<score_t> scoreList;

    QNetworkAccessManager *networkManager;
};

#endif // RANKING_H
