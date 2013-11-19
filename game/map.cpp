#include "map.h"
#include "gamebase.h"

#include "scripteventmanager.h"

#include <QtXml>


Map::Map()
{
    scriptEventManager = new ScriptEventManager();
}

Map::~Map()
{
    DESTROY_QVECTOR(layersBackground);
    DESTROY_QVECTOR(layersForeground);

    delete scriptEventManager;
}

void Map::Spawn()
{
    BaseClass::Spawn();

    scriptEventManager->InitEvents(GetGameContext());

    AudioManager::GetInstance()->PlayMusic(musicIdle.c_str());
}

void Map::OnRender(const render_context_t &context)
{
    FOREACH_QVECTOR_FAST(layersBackground, Layer*, l)
    {
        l->OnRender(context);
    }
    FOREACH_QVECTOR_FAST_END;
}

void Map::OnRenderForeground(const render_context_t &context)
{
    FOREACH_QVECTOR_FAST(layersForeground, Layer*, l)
    {
        l->OnRender(context);
    }
    FOREACH_QVECTOR_FAST_END;
}

void Map::OnSimulate(float frametime)
{
    FOREACH_QVECTOR_FAST(layersBackground, Layer*, l)
    {
        l->OnSimulate(frametime);
    }
    FOREACH_QVECTOR_FAST_END;

    FOREACH_QVECTOR_FAST(layersForeground, Layer*, l)
    {
        l->OnSimulate(frametime);
    }
    FOREACH_QVECTOR_FAST_END;

    scriptEventManager->OnSimulate(frametime);
}

void Map::OnEntityRemoved(Entity *entity)
{
    scriptEventManager->OnEntityRemoved(entity);
}

Map *Map::CreateMapFromXML(const char *path)
{
    Map *map = new Map();

    map->LoadFromXML(path);

    return map;
}

void Map::LoadFromXML(const char *path)
{
    QDomElement root;
    if (!OpenXMLFile(path, root))
    {
        DBGWARNING("!! Error loading map file:" << path);
        return;
    }

    QDomElement layerRoot = root.firstChildElement("layers");
    LoadLayers(layerRoot);

    scriptEventManager->LoadEventsFromXML(root.firstChildElement("events"));

    musicIdle = root.firstChildElement("music_idle").text().toStdString();
    musicBoss = root.firstChildElement("music_boss").text().toStdString();
}

void Map::LoadLayers(const QDomElement &root)
{
    QDomNodeList layerList = root.childNodes();

    FOREACH_QDOM_NODE(layerList, e)
    {
        QString tag = e.tagName();
        Layer *layer = NULL;

        if (tag.compare("fill", Qt::CaseInsensitive) == 0)
            layer = CreateFillLayer(e);
        else if (tag.compare("particle", Qt::CaseInsensitive) == 0)
            layer = CreateParticleLayer(e);
        else if (e.isComment())
            continue;
        else
            DBGWARNING("!! invalid layer node:" << tag);

        if (layer != NULL)
        {
            bool foreground = e.firstChildElement("foreground").text().toInt() != 0;
            if (foreground)
                layersForeground.push_back(layer);
            else
                layersBackground.push_back(layer);
        }
    }
    FOREACH_QDOM_NODE_END;
}

Layer *Map::CreateFillLayer(const QDomElement &root)
{
    QBrush *brush = NULL;
    QString subType = root.attribute("type");

    if (subType.compare("flat", Qt::CaseInsensitive) == 0)
    {
        brush = new QBrush(XMLParseColor(root.firstChildElement("color")));
    }
    else if (subType.compare("lineargradient", Qt::CaseInsensitive) == 0)
    {
        QLinearGradient gradient;

        QPointF start = XMLParsePointF(root.firstChildElement("start"));
        start.rx() = VALUE_PROPORTIONAL_X(start.x());
        start.ry() = VALUE_PROPORTIONAL(start.y());
        QPointF stop = XMLParsePointF(root.firstChildElement("stop"));
        stop.rx() = VALUE_PROPORTIONAL_X(stop.x());
        stop.ry() = VALUE_PROPORTIONAL(stop.y());

        gradient.setStart(start);
        gradient.setFinalStop(stop);

        QDomNodeList nodes = root.firstChildElement("colors").elementsByTagName("color");

        FOREACH_QDOM_NODE(nodes, n)
        {
            QColor color = XMLParseColor(n);
            float fraction = n.firstChildElement("fraction").text().toFloat();
            gradient.setColorAt(fraction, color);
        }
        FOREACH_QDOM_NODE_END;

        brush = new QBrush(gradient);
    }
    else if (subType.compare("radialgradient", Qt::CaseInsensitive) == 0)
    {
        QPointF center = XMLParsePointF(root.firstChildElement("center"));
        center.rx() = VALUE_PROPORTIONAL_X(center.x());
        center.ry() = VALUE_PROPORTIONAL(center.y());
        float radius = VALUE_PROPORTIONAL_X(
                    root.firstChildElement("radius").text().toFloat());

        QRadialGradient gradient(center, radius);
        QDomNodeList nodes = root.firstChildElement("colors").elementsByTagName("color");

        FOREACH_QDOM_NODE(nodes, n)
        {
            QColor color = XMLParseColor(n);
            float fraction = n.firstChildElement("fraction").text().toFloat();
            gradient.setColorAt(fraction, color);
        }
        FOREACH_QDOM_NODE_END;

        brush = new QBrush(gradient);
    }

    if (brush == NULL)
    {
        DBGWARNING("!! failed parsing layer");
        return NULL;
    }

    DBGWARNING("Creating fill layer:" << subType);

    Layer *l = new LayerFill(*brush);
    delete brush;
    return l;
}

Layer *Map::CreateParticleLayer(const QDomElement &root)
{
    LayerParticle *layer = new LayerParticle();

    QDomNodeList nodes = root.elementsByTagName("system");
    FOREACH_QDOM_NODE(nodes, n)
    {
        Vector2D origin = XMLParseVector(n.firstChildElement("origin"));
        std::string name = n.firstChildElement("name").text().toStdString();

        layer->AddParticle(name.c_str(), origin);
    }
    FOREACH_QDOM_NODE_END;

    return layer;
}
