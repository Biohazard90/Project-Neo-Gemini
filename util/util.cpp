#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QDebug>

#include <QFile>
#include <QtXml>
#include <QPainter>
#include <QCryptographicHash>

#include "vector2d.h"
#include "particle.h"
#include "util.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <stdio.h>
#include <lmcons.h>
#endif

void CenterOnScreen(QWidget *pWidget)
{
   // pWidget->adjustSize();
    pWidget->move(QApplication::desktop()->screen()->rect().center() - pWidget->rect().center());
}

QString OSUserName()
{
#ifdef PLATFORM_WINDOWS
    DWORD cchBuff = 256;
    TCHAR tchBuffer[UNLEN + 1];
    LPTSTR lpszSystemInfo = tchBuffer;

    GetUserName(lpszSystemInfo, &cchBuff);

    return QString::fromUtf16((const ushort*)lpszSystemInfo);
#endif
}

QString GetFileHash(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    if (!file.isOpen())
    {
        Q_ASSERT(0);
        return "error";
    }

    QByteArray data = file.readAll();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();

    file.close();

    return QString(hash);
}

QPainter::CompositionMode StringToCompositionMode(const QString &string)
{
    //OpenGL compatible modes
    if (string.compare("sourceover") == 0
            || string.isEmpty())
        return QPainter::CompositionMode_SourceOver;

    return QPainter::CompositionMode_Plus;

    // svg blend modes
    if (string.compare("plus", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Plus;
    else if (string.compare("multiply", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Multiply;
    else if (string.compare("screen", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Screen;
    else if (string.compare("overlay", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Overlay;
    else if (string.compare("darken", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Darken;
    else if (string.compare("lighten", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Lighten;
    else if (string.compare("colordodge", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_ColorDodge;
    else if (string.compare("colorburn", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_ColorBurn;
    else if (string.compare("hardlight", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_HardLight;
    else if (string.compare("softlight", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_SoftLight;
    else if (string.compare("difference", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Difference;
    else if (string.compare("exclusion", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Exclusion;
    // simple blend modes
    else if (string.compare("sourceover", Qt::CaseInsensitive) == 0
             || string.isEmpty())
        return QPainter::CompositionMode_SourceOver;
    else if (string.compare("destinationover", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_DestinationOver;
    else if (string.compare("clear", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Clear;
    else if (string.compare("source", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Source;
    else if (string.compare("destination", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Destination;
    else if (string.compare("sourcein", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_SourceIn;
    else if (string.compare("destinationin", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_DestinationIn;
    else if (string.compare("sourceout", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_SourceOut;
    else if (string.compare("destinationout", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_DestinationOut;
    else if (string.compare("sourceatop", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_SourceAtop;
    else if (string.compare("destinationatop", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_DestinationAtop;
    else if (string.compare("xor", Qt::CaseInsensitive) == 0)
        return QPainter::CompositionMode_Xor;
    else
    {
        DBGWARNING("!! Unknown blend mode:" << string);
        return QPainter::CompositionMode_SourceOver;
    }
}

ParticleField_e StringToParticleField(const QString &string)
{
    if (string.compare("origin", Qt::CaseInsensitive) == 0)
        return Field_origin;
    else if (string.compare("velocity", Qt::CaseInsensitive) == 0)
        return Field_velocity;
    else if (string.compare("angle", Qt::CaseInsensitive) == 0)
        return Field_angle;
    else if (string.compare("angularvelocity", Qt::CaseInsensitive) == 0)
        return Field_angularvelocity;
    else if (string.compare("size", Qt::CaseInsensitive) == 0)
        return Field_size;
    else if (string.compare("length", Qt::CaseInsensitive) == 0)
        return Field_length;
    else if (string.compare("color", Qt::CaseInsensitive) == 0)
        return Field_color;
    else if (string.compare("alpha", Qt::CaseInsensitive) == 0)
        return Field_alpha;
    else if (string.compare("lifetime", Qt::CaseInsensitive) == 0)
        return Field_lifetime;
    else
    {
        DBGWARNING("!! Unknown particle field:" << string);
        return Field_alpha;
    }
}

bool OpenXMLFile(const QString &path, QDomDocument &dest)
{
    QFile file(OSLocalPath(path));
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        DBGWARNING("!!Cannot read file " << qPrintable(path)
                  << ": " << qPrintable(file.errorString()));
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!dest.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn))
    {
        DBGWARNING("Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr));
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool OpenXMLFile(const QString &path, QDomElement &root)
{
    QDomDocument doc;
    if (!OpenXMLFile(path, doc))
    {
        return false;
    }
    root = doc.documentElement();
    return true;
}

QDomElement XMLFirstChildElementNoRecursion(const QDomElement &root, const QString &name)
{
    for (QDomElement n = root.firstChildElement();
         !n.isNull();
         n = n.nextSiblingElement())
    {
        if (n.tagName().compare(name, Qt::CaseInsensitive) == 0)
            return n;
    }

    return QDomElement();
}

QColor XMLParseColor(const QDomElement &node)
{
    QColor c(255, 255, 255, 255);
    QDomElement er = XMLFirstChildElementNoRecursion(node, "r");

    if (!er.isNull())
        c.setRed(er.text().toInt());

    er = XMLFirstChildElementNoRecursion(node, "g");
    if (!er.isNull())
        c.setGreen(er.text().toInt());

    er = XMLFirstChildElementNoRecursion(node, "b");
    if (!er.isNull())
        c.setBlue(er.text().toInt());

    er = XMLFirstChildElementNoRecursion(node, "a");
    if (!er.isNull())
        c.setAlpha(er.text().toInt());

    return c;
}

Vector2D XMLParseVector(const QDomElement &node)
{
    Vector2D v;
    v.x = XMLFirstChildElementNoRecursion(node, "x").text().toFloat();
    v.y = XMLFirstChildElementNoRecursion(node, "y").text().toFloat();
    return v;
}

QPoint XMLParsePoint(const QDomElement &node)
{
    QPoint v;
    v.rx() = XMLFirstChildElementNoRecursion(node, "x").text().toInt();
    v.ry() = XMLFirstChildElementNoRecursion(node, "y").text().toInt();
    return v;
}

QPointF XMLParsePointF(const QDomElement &node)
{
    QPointF v;
    v.rx() = XMLFirstChildElementNoRecursion(node, "x").text().toFloat();
    v.ry() = XMLFirstChildElementNoRecursion(node, "y").text().toFloat();
    return v;
}

bool XMLParseBoolean(const QDomElement &node, bool defaultValue)
{
    if (node.isNull() || node.text().isNull())
        return defaultValue;

    return node.text().compare("true", Qt::CaseInsensitive) == 0
            || node.text().toInt() != 0;
}

int XMLParseInt(const QDomElement &node, int defaultValue)
{
    if (node.isNull() || node.text().isNull())
        return defaultValue;

    return node.text().toInt();
}

float XMLParseFloat(const QDomElement &node, float defaultValue)
{
    if (node.isNull() || node.text().isNull())
        return defaultValue;

    return node.text().toFloat();
}

void XMLWriteString(QDomDocument &doc, QDomElement &parent, const QString &nodeName, const QString &value)
{
    QDomElement node = doc.createElement(nodeName);
    node.appendChild(doc.createTextNode(value));
    parent.appendChild(node);
}

void XMLWriteInt(QDomDocument &doc, QDomElement &parent, const QString &nodeName, int value)
{
    QDomElement node = doc.createElement(nodeName);
    node.appendChild(doc.createTextNode(QString::number(value)));
    parent.appendChild(node);
}

void XMLWriteFloat(QDomDocument &doc, QDomElement &parent, const QString &nodeName, float value)
{
    QDomElement node = doc.createElement(nodeName);
    node.appendChild(doc.createTextNode(QString("%1").arg(value)));
    parent.appendChild(node);
}
