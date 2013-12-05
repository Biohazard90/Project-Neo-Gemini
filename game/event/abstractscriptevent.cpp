#include "scripteventbase.h"

AbstractScriptEvent::AbstractScriptEvent()
    : root(nullptr)
    , parent(nullptr)
    , gameContext(nullptr)
{
}

void AbstractScriptEvent::deleteThis()
{
    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        e->deleteThis();
    }
    FOREACH_QLIST_END;

    delete this;
}

void AbstractScriptEvent::OnSimulate(float frametime)
{
    if (IsAutoFireEnabled() && ShouldFire())
    {
        FireEvent();
    }

    SimulateChildren(frametime);

    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        if (e->IsComplete())
        {
            children.removeOne(e);
            e->deleteThis();
        }
    }
    FOREACH_QLIST_END;
}

void AbstractScriptEvent::SimulateChildren(float frametime)
{
    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        e->OnSimulate(frametime);
    }
    FOREACH_QLIST_END;
}

void AbstractScriptEvent::ParseFromXML(const QDomElement &root)
{
    name = root.firstChildElement("name").text();

    FOREACH_QDOM_CHILD(root, event, node)
    {
        QString type = node.attribute("type");
        IScriptEvent *child = ScriptEventFactory::GetInstance()->CreateScriptEventByName(type);

        if (child == nullptr)
            continue;

        child->ParseFromXML(node);
        children.push_back(child);
    }
    FOREACH_QDOM_CHILD_END;
}

void AbstractScriptEvent::Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
{
    Q_ASSERT(gameContext != nullptr);
    Q_ASSERT(root != nullptr);

    this->gameContext = gameContext;
    this->root = root;
    this->parent = parent;

    FOREACH_QLIST(children, IScriptEvent*, child)
    {
        child->Init(gameContext, root, this);
    }
    FOREACH_QLIST_END;
}

const QString &AbstractScriptEvent::GetName() const
{
    return name;
}

IScriptEvent *AbstractScriptEvent::FindChildByName(const QString &name)
{
    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        if (e->GetName().compare(name, Qt::CaseInsensitive) == 0)
            return e;

        IScriptEvent *child = e->FindChildByName(name);
        if (child != nullptr)
            return child;
    }
    FOREACH_QLIST_END;

    return nullptr;
}

bool AbstractScriptEvent::ShouldFire() const
{
    return true;
}

bool AbstractScriptEvent::IsComplete() const
{
    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        if (!e->IsComplete())
            return false;
    }
    FOREACH_QLIST_END;

    return true;
}

void AbstractScriptEvent::OnEntityRemoved(Entity *entity)
{
    FOREACH_QLIST(children, IScriptEvent*, e)
    {
        e->OnEntityRemoved(entity);
    }
    FOREACH_QLIST_END;
}
