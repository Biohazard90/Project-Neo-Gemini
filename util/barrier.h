#ifndef BARRIER_H
#define BARRIER_H

#include <QSharedPointer>
#include <QMutex>
#include <QWaitCondition>

class Barrier
{
    Barrier(const Barrier &other);
public:
    Barrier(int count);

    void Wait();

    void Reset();

private:
    int m_count;
    const int m_count_initial;

    QMutex mutex;
    QWaitCondition event;
};

#endif // BARRIER_H
