#ifndef RANKING_H
#define RANKING_H

#include <QObject>
#include <QList>
#include <QString>

class Ranking : public QObject
{
    Q_OBJECT

public:
    static Ranking *GetInstance();    
    void LoadRanking();

    void Update();
    void Reset();
    void AddScore();

signals:
    void updateHighscore();

public slots:

    void onSubmit();
    int getScore(int i);
    QString getPlayer(int i);

private:
    Ranking();
    void WriteRanking();
    static Ranking instance;
    QString path;

    struct score_t
    {
        QString name;
        int score;
    };

    QList<score_t> scoreList;

    void SortRanking();
};

#endif // RANKING_H
