#include "abstractentityscriptevent.h"
#include "scripteventbase.h"

#include <QtXml>

AbstractEntityScriptEvent::AbstractEntityScriptEvent()
{
    origin_y_min = 0;
    origin_y_max = 1;
    origin_y_fixed = 0.5f;
    evenDistribution = false;
}

void AbstractEntityScriptEvent::ParseFromXML(const QDomElement &root)
{
    BaseClass::ParseFromXML(root);

    origin_y_min = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "origin_y_min"), origin_y_min);
    origin_y_max = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "origin_y_max"), origin_y_max);

    FOREACH_QDOM_CHILD(root, class, node)
    {
        classes.push_back(node.text());
    }
    FOREACH_QDOM_CHILD_END;
}

void AbstractEntityScriptEvent::FireEvent()
{
    Entity *entity = GetGameContext()->CreateEntityNoSpawn(GetEntityClassName());

    if (entity != NULL)
    {
        SpawnEntity(entity);

        GetGameContext()->SpawnEntity(entity);
    }
}

void AbstractEntityScriptEvent::FireEventEvenDistribution(int spawnIndex, int spawnCount)
{
    this->evenDistribution = true;

    const float stepSize = 1.0f / (spawnCount + 1) * (GetOriginYMax() - GetOriginYMin());
    origin_y_fixed = GetOriginYMin() + stepSize + spawnIndex * stepSize;

    FireEvent();

    this->evenDistribution = false;
}

float AbstractEntityScriptEvent::GetOriginYRandom()
{
    float fraction = 0.5f;

    if (evenDistribution)
    {
        fraction = origin_y_fixed;
    }
    else
    {
        fraction = qlerp(qfrand(), GetOriginYMin(), GetOriginYMax());
    }

    return qlerp(fraction,
                 Camera::GetInstance()->GetWorldMins().y,
                 Camera::GetInstance()->GetWorldMaxs().y);
}
