#include "ranking.h"
#include "util.h"
#include "score.h"

#include <QtXml>
#include <QTextStream>
#include <QFile>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCryptographicHash>

Ranking Ranking::instance;

Ranking::Ranking()
    : networkManager(nullptr)
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

void Ranking::onSubmit()
{
    Score::GetInstance()->Validate();

    PostHighscore(Score::GetInstance()->getName(), Score::GetInstance()->getScore());
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

void Ranking::PostHighscore(const QString &name, int score)
{
    CreateNetworkAccessManagerOnDemand();

    QString hashString(name.toUtf8() + " " + QString("%1").arg(score));
    QByteArray hash = QCryptographicHash::hash(hashString.toLocal8Bit(), QCryptographicHash::Md5).toHex();

    int scoreHashValue = 72;

    for (int i = 0; i < hash.length(); i++)
    {
        scoreHashValue += int(hash.at(i)) % 42;
    }

    hashString = hash + QString("%1").arg(scoreHashValue);

    hash = QCryptographicHash::hash(hashString.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    hashString = hash;

    QString postURL("http://bio2k.homepage.t-online.de/hig_game_design/posthighscore.php");

    QUrlQuery params;
    params.addQueryItem("scorename", name);
    params.addQueryItem("scorevalue", QString("%1").arg(score));
    params.addQueryItem("scorehash", hashString);

    QByteArray data = params.query().toUtf8();

    QUrl url(postURL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkManager->post(request, data);
}

void Ranking::RequestHighscores()
{
    CreateNetworkAccessManagerOnDemand();

    QString postURL("http://bio2k.homepage.t-online.de/hig_game_design/gethighscore.php");

    QUrlQuery params;
    QByteArray data = params.query().toUtf8();

    QUrl url(postURL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkManager->post(request, data);
}

void Ranking::CreateNetworkAccessManagerOnDemand()
{
    if (networkManager == nullptr)
    {
        networkManager = new QNetworkAccessManager(qApp);
        QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onScoreReceived(QNetworkReply *)));
    }
}

void Ranking::onScoreReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "!! Ranking network error: " << reply->errorString();
        return;
    }

    QString str = reply->readAll();

    QStringList list = str.split("\n");

    scoreList.clear();

    for (int i = 0; i < list.length(); i++)
    {
        if (list[i].length() < 1)
        {
            list.removeAt(i);
            i--;
        }
    }

    for (int i = 0; i < list.length(); i += 2)
    {
        score_t score;
        score.name = list[i].toLocal8Bit();
        score.score = list[i + 1].toInt();
        scoreList.append(score);
    }

    Update();
}




