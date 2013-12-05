#include "../base.h"
#include "events.h"

Events Events::instance;

Events::Events()
{
}

Events *Events::GetInstance()
{
    return &instance;
}

void Events::AddListener(const char *eventName, IEventListener *listener)
{
    if (!listeners.contains(eventName))
    {
        listeners.insert(eventName, QList<IEventListener *>());
    }

    auto &list = listeners[eventName];

    if (list.contains(listener))
    {
        Q_ASSERT(0);
        return;
    }

    list.append(listener);
}

void Events::RemoveListener(IEventListener *listener)
{
    for (auto &list : listeners)
    {
        list.removeOne(listener);
    }
}

void Events::FireEvent(KeyValues *data)
{
    const char *name = data->GetName();

    if (listeners.contains(name))
    {
        auto &list = listeners[name];

        for (auto listener : list)
        {
            listener->OnEvent(name, data);
        }
    }

    data->Release();
}
