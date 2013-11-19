#ifndef ABSTRACTENTITYSCRIPTEVENT_H
#define ABSTRACTENTITYSCRIPTEVENT_H

#include "abstractdelayedscriptevent.h"

class Entity;

class AbstractEntityScriptEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(AbstractEntityScriptEvent, AbstractScriptEvent);

public:
    AbstractEntityScriptEvent();

    virtual void ParseFromXML(const QDomElement &root);
    virtual void FireEvent();
    virtual void FireEventEvenDistribution(int spawnIndex, int spawnCount);

protected:
    virtual bool IsAutoFireEnabled() const { return false; }

    const QList<QString> &GetClasses() const { return classes; }

    virtual const char *GetEntityClassName() = 0;
    virtual void SpawnEntity(Entity *entity) = 0;

    virtual float GetOriginYMin() const { return origin_y_min; }
    virtual float GetOriginYMax() const { return origin_y_max; }

    virtual float GetOriginYRandom();

private:
    float origin_y_min;
    float origin_y_max;

    bool evenDistribution;
    float origin_y_fixed;

    QList<QString> classes;
};

#endif // ABSTRACTENTITYSCRIPTEVENT_H
