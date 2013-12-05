#ifndef EVENTS_H
#define EVENTS_H

#include <QHash>

class IEventListener
{
protected:
    virtual ~IEventListener() {}
public:
    virtual void OnEvent(const char *name, KeyValues *data) = 0;
};

class Events
{
    Events();
    static Events instance;
public:

    static Events *GetInstance();

    void AddListener(const char *eventName, IEventListener *listener);
    void RemoveListener(IEventListener *listener);

    // takes ownership
    void FireEvent(KeyValues *data);

private:

    QHash<QString, QList<IEventListener *>> listeners;
};

#endif // EVENTS_H
