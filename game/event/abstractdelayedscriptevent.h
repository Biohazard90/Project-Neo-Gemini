#ifndef ABSTRACTDELAYEDSCRIPTEVENT_H
#define ABSTRACTDELAYEDSCRIPTEVENT_H

#include "abstractscriptevent.h"

class AbstractDelayedScriptEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(AbstractDelayedScriptEvent, AbstractScriptEvent);

public:
    AbstractDelayedScriptEvent();

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent);
    virtual void ParseFromXML(const QDomElement &root);

    virtual bool ShouldFire() const;
    virtual void FireEvent();
    virtual bool IsComplete() const;

protected:
    bool HasFired() const { return fired; }
    float GetStartTime() const { return startTime; }

private:
    bool fired;
    float startTime;
};

#endif // ABSTRACTDELAYEDSCRIPTEVENT_H
