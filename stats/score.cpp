#include "score.h"

#include <QCryptographicHash>

Score Score::instance;

Score::Score()
{
   score = 0;
   scoreHash = 0;
   name = "";
}

Score *Score::GetInstance()
{
    return &instance;
}

void Score::Reset()
{
    setScore(0);
}

void Score::AddPoints(int points)
{
    setScore(getScore() + points);
}

void Score::setName(const QString &name)
{
    this->name = name;
    emit nameChanged(name);
}

void Score::setScore(int score)
{
    Validate();

    this->score = score;
    emit scoreChanged(score);

    scoreHash = BuildScoreHash(score);
}

void Score::Validate()
{
    if (score == 0)
    {
        return;
    }

    int currentHash = BuildScoreHash(score);

    if (currentHash != scoreHash)
    {
        qCritical() << "Bye.";
        std::abort();
    }
}

int Score::BuildScoreHash(int value)
{
    return value % 3 + value % 5 + value % 7
            - value;
}
