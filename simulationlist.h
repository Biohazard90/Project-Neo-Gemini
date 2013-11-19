#ifndef SIMULATIONLIST_H
#define SIMULATIONLIST_H

#include <QObject>
#include <QList>
#include <QElapsedTimer>

#include "isimulated.h"

class SimulationList : public QObject
{
    Q_OBJECT
    static SimulationList instance;
    SimulationList();
public:

    static SimulationList *GetInstance();

    static void AddSimulationObject(ISimulated *object);
    static void RemoveSimulationObject(ISimulated *object);

    void SimulateAll(float frametime);

public slots:
    void OnFrame();

private:
    QElapsedTimer m_timer;
    qint64 m_iLastTime;

    static QList<ISimulated*> m_simulationList;

    int tickcount;
    float tickframe;
};

#endif // SIMULATIONLIST_H
