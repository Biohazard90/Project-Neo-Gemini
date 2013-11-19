#ifndef SCRIPTEVENTMANAGER_H
#define SCRIPTEVENTMANAGER_H

#include "isimulated.h"

class QDomElement;
class IScriptEvent;
class Entity;
class IGameContext;

class ScriptEventManager : public ISimulated
{
public:
    ScriptEventManager();
    ~ScriptEventManager();

    virtual void OnSimulate(float frametime);

    void LoadEventsFromXML(const QDomElement &root);

    void InitEvents(IGameContext *gameContext);

    void OnEntityRemoved(Entity *entity);

private:
    IScriptEvent *rootEvent;
};

#endif // SCRIPTEVENTMANAGER_H
