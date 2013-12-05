#include "simulationlist.h"
#include "base.h"

SimulationList SimulationList::instance;
QList<ISimulated*> SimulationList::m_simulationList;

SimulationList::SimulationList()
    : QObject(nullptr)
{
    m_timer.start();
    m_iLastTime = 0;
    tickcount = 0;
    tickframe = 0.0f;
}

SimulationList *SimulationList::GetInstance()
{
    return &instance;
}

void SimulationList::AddSimulationObject(ISimulated *object)
{
    m_simulationList.push_back(object);
}

void SimulationList::RemoveSimulationObject(ISimulated *object)
{
    m_simulationList.removeOne(object);
}

void SimulationList::SimulateAll(float frametime)
{
    QListIterator<ISimulated*> itr(m_simulationList);
    while(itr.hasNext())
    {
        ISimulated *p = itr.next();
        p->OnSimulate(frametime);
    }
}

void SimulationList::OnFrame()
{
    float frametime = 0.0f;

    if (m_timer.isValid())
    {
        qint64 time = m_timer.nsecsElapsed();
        qint64 delta = (time - m_iLastTime) / 1000;
        frametime = delta / 1000000.0f;
        m_iLastTime = time;

        if (frametime > 0.1f)
        {
            frametime = 0.1f;
        }

        pGlobals->frametime = frametime;

        tickframe += frametime;
        while ( tickframe > 1.0f )
        {
            tickcount++;
            tickframe -= 1.0f;
        }

        pGlobals->curtime = tickcount * 1.0f + tickframe;
    }

    SimulateAll(frametime);
}
