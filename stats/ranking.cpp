#include "ranking.h"
#include "util.h"
#include "score.h"

#include <QtXml>
#include <QTextStream>
#include <QFile>
#include <QXmlStreamWriter>
#include <QDebug>

Ranking Ranking::instance;

Ranking::Ranking()
{
    path = QString(PATH_SCORE_ROOT);
    path += "score.xml";
}

Ranking *Ranking::GetInstance()
{
    return &instance;
}

void Ranking::Update()
{
    emit updateHighscore();
}

void Ranking::AddScore()
{
    Score *tmpScore = Score::GetInstance();
    score_t newScore;
    newScore.name = tmpScore->getName();
    newScore.score = tmpScore->getScore();

    if (newScore.name.isEmpty())
        newScore.name = "Unnamed";

    scoreList.push_back(newScore);
    SortRanking();
    while (scoreList.size() > 5)
        scoreList.removeLast();

    Update();
}

void Ranking::onSubmit()
{
    AddScore();
    WriteRanking();
}

void Ranking::SortRanking()
{
    for(int i = 0; i < scoreList.size();i++)
    {
        for(int j = i+1; j < scoreList.size(); j++)
        {
            if(scoreList[i].score < scoreList[j].score)
            {
                score_t tmp = scoreList[i];
                scoreList[i] = scoreList[j];
                scoreList[j] = tmp;
            }
        }
    }
}

void Ranking::LoadRanking()
{
    QDomElement root;
    if (!OpenXMLFile(path, root)){
        return;
    }
    QDomNodeList result = root.childNodes();

    FOREACH_QDOM_NODE(result, e)
    {
        score_t newScore;
        newScore.name = e.firstChildElement("name").text();
        newScore.score = e.firstChildElement("points").text().toInt();
        scoreList.push_back(newScore);
    }
    FOREACH_QDOM_NODE_END;

    SortRanking();
}

void Ranking::WriteRanking()
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
    xmlWriter->setDevice(&file);
    xmlWriter->writeStartElement("scores");

    for(int i = 0; i < scoreList.size(); i++)
    {
        xmlWriter->writeStartElement("score");
        xmlWriter->writeTextElement("name", scoreList[i].name);
        xmlWriter->writeTextElement("points", QString::number(scoreList[i].score));
        xmlWriter->writeEndElement();
    }
    xmlWriter->writeEndElement();
    delete xmlWriter;
}

int Ranking::getScore(int i)
{
    int result = -1;
    if(scoreList.size() > i)
        result = scoreList[i].score;
    return result;
}

QString Ranking::getPlayer(int i)
{
    QString result = "";
    if(scoreList.size() > i)
        result = scoreList[i].name;
    return result;
}




