#include "scripteventbase.h"

class ObstacleEvent : public AbstractEntityScriptEvent
{
    DECLARE_CLASS(ObstacleEvent, AbstractEntityScriptEvent);

    QList<Resource_Obstacle_t> data;

public:

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);

        FOREACH_QLIST(GetClasses(), QString, c)
        {
            data.push_back(Resource::GetInstance()->GetObstacle(c));
        }
        FOREACH_QLIST_END;
    }

    virtual const char *GetEntityClassName()
    {
        return "obstacle";
    }

    virtual void SpawnEntity(Entity *entity)
    {
        Obstacle *obstacle = (Obstacle*)entity;

        Q_ASSERT(data.size() > 0);

        Resource_Obstacle_t info = data.at(qrand(data.size() - 1));
        obstacle->Init(info);

        Vector2D origin = obstacle->GetOrigin();
        origin.y = GetOriginYRandom();
        obstacle->Teleport(origin);
    }

private:

};

REGISTER_SCRIPT_EVENT(ObstacleEvent, obstacle);
