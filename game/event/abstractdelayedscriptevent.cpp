#include "scripteventbase.h"

AbstractDelayedScriptEvent::AbstractDelayedScriptEvent()
    : fired(false)
    , startTime(0)
{
}

void AbstractDelayedScriptEvent::Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
{
    BaseClass::Init(gameContext, root, parent);

    startTime += gameContext->GetGameTime();
}

void AbstractDelayedScriptEvent::ParseFromXML(const QDomElement &root)
{
    BaseClass::ParseFromXML(root);

    startTime = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "delay"), startTime);
}

bool AbstractDelayedScriptEvent::ShouldFire() const
{
    if (GetGameContext()->GetGameTime() < startTime)
        return false;

    return BaseClass::ShouldFire();
}

void AbstractDelayedScriptEvent::FireEvent()
{
    fired = true;
}

bool AbstractDelayedScriptEvent::IsComplete() const
{
    return fired && BaseClass::IsComplete();
}
