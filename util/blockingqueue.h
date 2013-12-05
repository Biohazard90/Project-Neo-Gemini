#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include "platform.h"

#include <QSharedPointer>
#include <QMutex>
#include <QWaitCondition>

template< class T >
class BlockingQueue
{
public:
    BlockingQueue(int size);
    ~BlockingQueue();

    T Take();
    void Put( T value );

private:
    QMutex mutex;
    QWaitCondition event;

    int m_size;
    int m_size_used;

    T *base;

    T *write;
    T *read;
};

template< class T >
BlockingQueue< T >::BlockingQueue(int size)
    : m_size( size )
    , m_size_used( 0 )
{
    base = new T[ size ];
    write = base;
    read = base;
}

template< class T >
BlockingQueue< T >::~BlockingQueue()
{
    delete [] base;
}

template< class T >
void BlockingQueue< T >::Put( T value )
{
    mutex.lock();

    while ( m_size_used >= m_size )
    {
        event.wait(&mutex);
    }

    Q_ASSERT(m_size_used < m_size);

    *write = value;

    write++;
    m_size_used++;

    if ((write - base) >= m_size)
        write = base;

    event.wakeAll();

    mutex.unlock();
}

template< class T >
T BlockingQueue< T >::Take()
{
    T ret = nullptr;

    mutex.lock();

    while ( m_size_used <= 0 )
    {
        event.wait(&mutex);
    }

    Q_ASSERT(m_size_used > 0);

    ret = *read;

    read++;
    m_size_used--;

    if ((read - base) >= m_size)
        read = base;

    event.wakeAll();

    mutex.unlock();

    return ret;
}

#endif // BLOCKINGQUEUE_H
