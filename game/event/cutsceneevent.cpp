#include "scripteventbase.h"

#include <string>

class CutsceneEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(CutsceneEvent, AbstractScriptEvent);

    QString portraitLeft;
    QString portraitRight;
    QString titel;
    QString message;

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);

        portraitLeft = QString::fromStdString(XMLFirstChildElementNoRecursion(root, "portraitLeft").text().toStdString());
        portraitRight = QString::fromStdString(XMLFirstChildElementNoRecursion(root, "portraitRight").text().toStdString());
        titel = QString::fromStdString(XMLFirstChildElementNoRecursion(root, "titel").text().toStdString());
        message = QString::fromStdString(XMLFirstChildElementNoRecursion(root, "message").text().toStdString());
    }


    virtual void FireEvent()
    {
        GetGameContext()->ShowCutscene(portraitLeft, portraitRight, titel, message);
    }

    virtual bool IsAutoFireEnabled() const { return false; }

};

REGISTER_SCRIPT_EVENT(CutsceneEvent, cutscene);
