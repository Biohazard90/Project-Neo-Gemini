#include "scripteventbase.h"

#include <string>

class MusicEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(MusicEvent, AbstractScriptEvent);

    bool looped;
    std::string filename;

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);

        filename = XMLFirstChildElementNoRecursion(root, "filename").text().toStdString();
        looped = XMLParseBoolean(XMLFirstChildElementNoRecursion(root, "looped"), true);
    }

    virtual void FireEvent()
    {
        AudioManager::GetInstance()->PlayMusic(filename.c_str(), 1.0f, looped);

    }

    virtual bool IsAutoFireEnabled() const { return false; }

};

REGISTER_SCRIPT_EVENT(MusicEvent, music);
