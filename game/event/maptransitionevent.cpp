#include "scripteventbase.h"

#include <string>

class MapTransitionEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(MapTransitionEvent, AbstractScriptEvent);

    QString mapname;

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);

        mapname = XMLFirstChildElementNoRecursion(root, "mapname").text();
    }

    virtual void FireEvent()
    {
        GetGameContext()->EndMap(mapname);
    }

    virtual bool IsAutoFireEnabled() const { return false; }

};

REGISTER_SCRIPT_EVENT(MapTransitionEvent, maptransition);

