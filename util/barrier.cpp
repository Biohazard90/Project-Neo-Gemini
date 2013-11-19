#include "barrier.h"

Barrier::Barrier(int count)
    : m_count( count )
    , m_count_initial( count )
{
}

void Barrier::Wait()
{
    mutex.lock();

    m_count--;

    if ( m_count > 0 )
        event.wait( &mutex );
    else
        event.wakeAll();

    mutex.unlock();
}

void Barrier::Reset()
{
    mutex.lock();

    m_count = m_count_initial;

    mutex.unlock();
}
