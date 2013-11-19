#include "scripteventbase.h"

#include "aifactory.h"

class BossEvent : public AbstractEntityScriptEvent
{
    DECLARE_CLASS(BossEvent, AbstractEntityScriptEvent);

    QList<Resource_Fighter_t> data;

    Entity *bossEntity;
    bool bossDied;

    float gameOverTimer;

public:

    BossEvent()
        : bossEntity(NULL)
        , bossDied(false)
        , gameOverTimer(0.0f)
    {

    }

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
        return "boss";
    }

    virtual void SpawnEntity(Entity *entity)
    {
        BossShip *fighter = (BossShip*)entity;

        Q_ASSERT(data.size() > 0);

        Resource_Fighter_t info = data.at(qrand(data.size() - 1));

        IAI *ai = AIFactory::GetInstance()->CreateAIByName(info.ai.c_str());

        fighter->Init(info, ai);

        Vector2D origin = fighter->GetOrigin();
        origin.y = qlerp(0.5f,
                                Camera::GetInstance()->GetWorldMins().y,
                                Camera::GetInstance()->GetWorldMaxs().y);
        fighter->Teleport(origin);

        fighter->SetAngle(-180);

        bossEntity = fighter;
    }

    virtual void OnSimulate(float frametime)
    {
        BaseClass::OnSimulate(frametime);

        if (gameOverTimer > 0.0f
                && gameOverTimer < GetGameContext()->GetGameTime())
        {
            gameOverTimer = 0.0f;
            GetGameContext()->EndMap();
        }
    }

    virtual void OnEntityRemoved(Entity *entity)
    {
        BaseClass::OnEntityRemoved(entity);

        if (entity == bossEntity)
        {
            bossEntity = NULL;
            bossDied = true;

            gameOverTimer = GetGameContext()->GetGameTime() + 3.0f;
        }
    }

    virtual bool ShouldFire() const
    {
        if (bossEntity != NULL
                || bossDied)
            return false;

        return BaseClass::ShouldFire();
    }

    virtual bool IsComplete() const
    {
        if (bossEntity != NULL)
            return false;

        if (gameOverTimer > 0.0f)
            return false;

        return bossDied && BaseClass::IsComplete();
    }

};

REGISTER_SCRIPT_EVENT(BossEvent, boss);
