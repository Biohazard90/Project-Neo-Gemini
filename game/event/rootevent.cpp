#include "scripteventbase.h"

class RootEvent : public AbstractScriptEvent
{
public:
    virtual bool ShouldFire() const
    {
        return false;
    }

    virtual bool IsComplete() const
    {
        return false;
    }

    virtual void FireEvent()
    {
    }
};

REGISTER_SCRIPT_EVENT(RootEvent, root);
