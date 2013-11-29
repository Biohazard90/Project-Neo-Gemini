#include "config.h"

#include "base.h"

#include <QtXml>
#include <QTextStream>
#include <QFile>

void Config::LoadConfig(Globals *globals)
{
    QDomElement root;
    if (!OpenXMLFile("content/config.xml", root))
        return;

    globals->antialiasing = root.firstChildElement("antialiasing").toElement().text().toInt() != 0;
    globals->showfps = root.firstChildElement("showfps").toElement().text().toInt() != 0;
    globals->musicenabled = root.firstChildElement("musicenabled").toElement().text().toInt() != 0;
    globals->screen_width = root.firstChildElement("screen_width").toElement().text().toInt();
    globals->screen_height = root.firstChildElement("screen_height").toElement().text().toInt();
    globals->fullscreen = root.firstChildElement("fullscreen").toElement().text().toInt() != 0;

    AudioManager::GetInstance()->SetMusicEnabled(globals->musicenabled);
}

void Config::SaveConfig(Globals *globals)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);

    QDomElement eAntialiasing = doc.createElement("antialiasing");
    eAntialiasing.appendChild(doc.createTextNode(QString::number(globals->antialiasing ? 1 : 0)));
    root.appendChild(eAntialiasing);

    QDomElement eShowfps = doc.createElement("showfps");
    eShowfps.appendChild(doc.createTextNode(QString::number(globals->showfps ? 1 : 0)));
    root.appendChild(eShowfps);

    QDomElement eMusicenabled = doc.createElement("musicenabled");
    eMusicenabled.appendChild(doc.createTextNode(QString::number(globals->musicenabled ? 1 : 0)));
    root.appendChild(eMusicenabled);

    QDomElement eFullscreenenabled = doc.createElement("fullscreen");
    eFullscreenenabled.appendChild(doc.createTextNode(QString::number(globals->fullscreen ? 1 : 0)));
    root.appendChild(eFullscreenenabled);

    QDomElement eScreenWidth = doc.createElement("screen_width");
    eScreenWidth.appendChild(doc.createTextNode(QString::number(globals->screen_width)));
    root.appendChild(eScreenWidth);

    QDomElement eScreenHeight = doc.createElement("screen_height");
    eScreenHeight.appendChild(doc.createTextNode(QString::number(globals->screen_height)));
    root.appendChild(eScreenHeight);

    QFile file(OSLocalPath("content/config.xml"));
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    doc.save(out, 4);

    file.close();
}
