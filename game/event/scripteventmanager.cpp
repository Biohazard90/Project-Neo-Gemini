#include "scripteventmanager.h"

#include "gamebase.h"

#include "iscriptevent.h"
#include "scripteventfactory.h"

#include <QtXml>

ScriptEventManager::ScriptEventManager()
{
    rootEvent = nullptr;
}

ScriptEventManager::~ScriptEventManager()
{
    if (rootEvent != nullptr)
        rootEvent->deleteThis();
}

void ScriptEventManager::LoadEventsFromXML(const QDomElement &root)
{
    if (rootEvent != nullptr)
        rootEvent->deleteThis();

    ScriptEventFactory::GetInstance()->Init();
    rootEvent = ScriptEventFactory::GetInstance()->CreateScriptEventByName("root");

    if (rootEvent != nullptr)
        rootEvent->ParseFromXML(root);
}

void ScriptEventManager::InitEvents(IGameContext *gameContext)
{
    if (rootEvent != nullptr)
        rootEvent->Init(gameContext, rootEvent, nullptr);
}

void ScriptEventManager::OnEntityRemoved(Entity *entity)
{
    if (rootEvent != nullptr)
        rootEvent->OnEntityRemoved(entity);
}

void ScriptEventManager::OnSimulate(float frametime)
{
    if (rootEvent != nullptr)
        rootEvent->OnSimulate(frametime);
}
