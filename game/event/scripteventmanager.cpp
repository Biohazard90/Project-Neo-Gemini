#include "scripteventmanager.h"

#include "gamebase.h"

#include "iscriptevent.h"
#include "scripteventfactory.h"

#include <QtXml>

ScriptEventManager::ScriptEventManager()
{
    rootEvent = NULL;
}

ScriptEventManager::~ScriptEventManager()
{
    if (rootEvent != NULL)
        rootEvent->deleteThis();
}

void ScriptEventManager::LoadEventsFromXML(const QDomElement &root)
{
    if (rootEvent != NULL)
        rootEvent->deleteThis();

    ScriptEventFactory::GetInstance()->Init();
    rootEvent = ScriptEventFactory::GetInstance()->CreateScriptEventByName("root");

    if (rootEvent != NULL)
        rootEvent->ParseFromXML(root);
}

void ScriptEventManager::InitEvents(IGameContext *gameContext)
{
    if (rootEvent != NULL)
        rootEvent->Init(gameContext, rootEvent, NULL);
}

void ScriptEventManager::OnEntityRemoved(Entity *entity)
{
    if (rootEvent != NULL)
        rootEvent->OnEntityRemoved(entity);
}

void ScriptEventManager::OnSimulate(float frametime)
{
    if (rootEvent != NULL)
        rootEvent->OnSimulate(frametime);
}
