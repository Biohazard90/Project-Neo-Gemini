#include "score.h"

Score Score::instance;

Score::Score()
{
   score = 0;
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
    this->score = score;
    emit scoreChanged(score);
}
