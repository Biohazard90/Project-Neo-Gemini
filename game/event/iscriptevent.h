#ifndef ISCRIPTEVENT_H
#define ISCRIPTEVENT_H

#include "macros.h"
#include "isimulated.h"

class QDomElement;
class IGameContext;
class Entity;

class IScriptEvent : public ISimulated
{
protected:
    virtual ~IScriptEvent(){}
public:

    virtual void deleteThis() = 0;

    // ISimulated :: virtual void OnSimulate(float frametime) = 0;

    virtual void ParseFromXML(const QDomElement &root) = 0;

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent) = 0;
    virtual const QString &GetName() const = 0;
    virtual IScriptEvent *FindChildByName(const QString &name) = 0;

    virtual bool ShouldFire() const = 0;
    virtual void FireEvent() = 0;
    virtual bool IsComplete() const = 0;

    virtual void OnEntityRemoved(Entity *entity) = 0;
};

#endif // ISCRIPTEVENT_H
