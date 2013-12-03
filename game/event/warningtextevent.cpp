#include "scripteventbase.h"

#include <string>

class WarningTextEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(WarningTextEvent, AbstractScriptEvent);

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);
    }

    virtual void FireEvent()
    {
        GetGameContext()->ShowWarningText();
    }

    virtual bool IsAutoFireEnabled() const { return false; }

};

REGISTER_SCRIPT_EVENT(WarningTextEvent, warningtext);

