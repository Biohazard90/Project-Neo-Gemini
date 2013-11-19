
#include "scripteventbase.h"

#include "aifactory.h"

class FighterEvent : public AbstractEntityScriptEvent
{
    DECLARE_CLASS(FighterEvent, AbstractEntityScriptEvent);

    QList<Resource_Fighter_t> data;

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);

        FOREACH_QLIST(GetClasses(), QString, c)
        {
            data.push_back(Resource::GetInstance()->GetFighter(c));
        }
        FOREACH_QLIST_END;
    }

    virtual const char *GetEntityClassName()
    {
        return "fighter";
    }

    virtual void SpawnEntity(Entity *entity)
    {
        EnemyShip *fighter = (EnemyShip*)entity;

        Q_ASSERT(data.size() > 0);

        Resource_Fighter_t info = data.at(qrand(data.size() - 1));

        IAI *ai = AIFactory::GetInstance()->CreateAIByName(info.ai.c_str());

        fighter->Init(info, ai);

        Vector2D origin = fighter->GetOrigin();
        origin.y = GetOriginYRandom();
        fighter->Teleport(origin);

        fighter->SetAngle(-180);
    }

private:

};

REGISTER_SCRIPT_EVENT(FighterEvent, fighter);
