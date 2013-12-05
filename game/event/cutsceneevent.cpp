#include "scripteventbase.h"

#include <string>

class CutsceneEvent : public AbstractScriptEvent
{
    DECLARE_CLASS(CutsceneEvent, AbstractScriptEvent);

    QString portraitLeft;
    QString portraitRight;
    QString titel;
    QString message;
    //TODO Yeah, thats ugly
    std::string soundClip;

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
        soundClip =  XMLFirstChildElementNoRecursion(root, "soundclip").text().toStdString();

    }


    virtual void FireEvent()
    {
        if(!soundClip.empty()){

            soundClip = "misc/" + soundClip;
            AudioManager::GetInstance()->PlaySoundSample(soundClip.c_str(), 1.0f);
        }

        GetGameContext()->ShowCutscene(portraitLeft, portraitRight, titel, message);

    }

    virtual bool IsAutoFireEnabled() const { return false; }

};

REGISTER_SCRIPT_EVENT(CutsceneEvent, cutscene);
