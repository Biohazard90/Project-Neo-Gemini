#ifndef SCORE_H
#define SCORE_H

#include <QObject>
#include <QString>

class Score : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int score READ getScore NOTIFY scoreChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

    Score();
    static Score instance;
public:
    static Score *GetInstance();

    void Reset();
    void AddPoints(int points);

    int getScore(){ return score; }
    QString getName(){ return name; }

    void setScore(int score);
    void setName(const QString &name);

    void Validate();
    
signals:
    void scoreChanged(int);
    void nameChanged(QString);
    
public slots:

private:
    int BuildScoreHash(int value);

    int score;
    int scoreHash;

    QString name;
};

#endif // SCORE_H

