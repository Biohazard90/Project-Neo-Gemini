#ifndef PARALLELRENDERER_H
#define PARALLELRENDERER_H

#include <QMetaType>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>

#include "queuedpainter.h"
#include "barrier.h"
#include "blockingqueue.h"
#include "macros.h"

class QThreadPool;

#define PARALLEL_BUFFER_SIZE (1 << 19)
#define PARALLEL_QUEUE_SIZE 256
#define PARALLEL_MAX_WORKERS 16

class ParallelRenderer;

class ParallelRenderWorker : public QRunnable
{
public:
    ParallelRenderWorker(int index, ParallelRenderer *parent);

    void run();

private:
    FORCEINLINE bool IsMaster(){ return index == 0; }
    FORCEINLINE bool IsSlave(){ return !IsMaster(); }

    int index;
    ParallelRenderer *m_parent;
};

class ParallelRenderer : public QObject
{
    Q_OBJECT
    friend class ParallelRenderWorker;
public:
    ParallelRenderer();
    ~ParallelRenderer();

    QueuedPainter *BeginJob();
    void EndJob();

    void SwapBuffers();

    FORCEINLINE int GetBufferOffset(int index)
    {
        Q_ASSERT(index >= 0 && index < threadCount);
        return bufferOffset[index];
    }

    FORCEINLINE int GetBufferWidth(int index)
    {
        Q_ASSERT(index >= 0 && index < threadCount);
        return bufferWidth[index];
    }

    FORCEINLINE QImage *GetBuffer(int index)
    {
        Q_ASSERT(index >= 0 && index < threadCount);
        return frontBuffer[index];
    }

    FORCEINLINE int GetBufferCount()
    {
        return threadCount;
    }

    FORCEINLINE float GetFrametimeRender()
    {
        float ft = 1;
        for (int i = 0; i < threadCount; i++ )
            ft = MIN(ft, frameTimeRender[i]);
        return ft;
    }

    FORCEINLINE float GetFrametimeSwap()
    {
        return frameTimeSwap;
    }

signals:

private:
    QThreadPool *threadPool;
    int threadCount;

    int bufferWidth[PARALLEL_MAX_WORKERS];
    int bufferHeight;
    int bufferOffset[PARALLEL_MAX_WORKERS];

    void StartWorkers();
    void StopWorkers();
    void OnWorkerFinished(int worker);

    QImage **frontBuffer;
    QImage **swapBuffer;
    QImage **backBuffer;
    bool swapBufferDirty[PARALLEL_MAX_WORKERS];

    char *queueMemory;
    char **swapQueueMemory;
    char **backQueueMemory;

    QueuedPainter frontPainter;
    QueuedPainter swapPainterWorker[PARALLEL_MAX_WORKERS];
    QueuedPainter backPainterWorker[PARALLEL_MAX_WORKERS];

    QMutex mutex_queue[PARALLEL_MAX_WORKERS];
    QMutex mutex_buffer[PARALLEL_MAX_WORKERS];

    volatile bool stopWorkers;


    FORCEINLINE float CalcFrameTime(qint64 &lastTime)
    {
        if (timer.isValid())
        {
            qint64 time = timer.nsecsElapsed();
            qint64 delta = (time - lastTime) / 1000;
            lastTime = time;
            return delta / 1000000.0f;
        }
        return 1.0f;
    }

    QElapsedTimer timer;
    qint64 lastTimeRender[PARALLEL_MAX_WORKERS];
    qint64 lastTimeSwap;
    float frameTimeRender[PARALLEL_MAX_WORKERS];
    float frameTimeSwap;
};

#endif // PARALLELRENDERER_H
