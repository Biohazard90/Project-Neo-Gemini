#ifndef ABSTRACTSCRIPTEVENT_H
#define ABSTRACTSCRIPTEVENT_H

#include <QList>
#include <QString>

#include "iscriptevent.h"


class AbstractScriptEvent : public IScriptEvent
{
public:
    AbstractScriptEvent();

    virtual void deleteThis();

    virtual void OnSimulate(float frametime);

    virtual void ParseFromXML(const QDomElement &root);

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent);
    virtual const QString &GetName() const;
    virtual IScriptEvent *FindChildByName(const QString &name);

    virtual bool ShouldFire() const;
    // virtual void FireEvent();
    virtual bool IsComplete() const;

    virtual void OnEntityRemoved(Entity *entity);

protected:
    IScriptEvent *GetRootEvent() const { return root; }
    IScriptEvent *GetParentEvent() const { return parent; }
    IGameContext *GetGameContext() const { return gameContext; }

    const QList<IScriptEvent*> &GetChildren() const { return children; }

    virtual bool IsAutoFireEnabled() const { return true; }
    virtual void SimulateChildren(float frametime);

private:
    IScriptEvent *root;
    IScriptEvent *parent;
    IGameContext *gameContext;
    QString name;

    QList<IScriptEvent*> children;

};

#endif // ABSTRACTSCRIPTEVENT_H
