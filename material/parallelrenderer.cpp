
#include <QThread>
#include <QThreadPool>
#include <QFont>

#include "macros.h"
#include "platform_warnings.h"
#include "globals.h"

#include "queuedpainter.h"
#include "passthroughpainter.h"
#include "parallelrenderer.h"

ParallelRenderWorker::ParallelRenderWorker(int index, ParallelRenderer *parent)
    : m_parent( parent )
{
    this->index = index;

}

void ParallelRenderWorker::run()
{
    while (!m_parent->stopWorkers)
    {
        m_parent->mutex_queue[index].lock();
        m_parent->swapPainterWorker[index].Swap(m_parent->backPainterWorker[index]);
        m_parent->mutex_queue[index].unlock();

        if (m_parent->backPainterWorker[index].CanExecute())
        {
            QPainter imgPainter(m_parent->backBuffer[index]);
            imgPainter.translate(-m_parent->bufferOffset[index], 0);
            PassthroughPainter passThroughPainter(imgPainter);

            m_parent->backPainterWorker[index].ExecuteOnDevice(passThroughPainter);

            m_parent->mutex_buffer[index].lock();
            qSwap(m_parent->backBuffer[index], m_parent->swapBuffer[index]);
            m_parent->swapBufferDirty[index] = true;

            m_parent->OnWorkerFinished(index);
            m_parent->mutex_buffer[index].unlock();
        }
        else
        {
            QThread::currentThread()->yieldCurrentThread();
        }
    }
}

ParallelRenderer::ParallelRenderer()
    : threadPool( nullptr )
    , lastTimeSwap( 0 )
    , frameTimeSwap( 1 )
    , stopWorkers( false )
{
    memset(swapBufferDirty, 0, sizeof(swapBufferDirty));
    for (int i = 0; i < PARALLEL_MAX_WORKERS; i++)
    {
        lastTimeRender[i] = 0;
        frameTimeRender[i] = 1;
    }

    threadCount = CLAMP(QThread::idealThreadCount() - 1, 1, PARALLEL_MAX_WORKERS);
    DBGMSG("Parallel worker count:" << threadCount);

    bufferHeight = pGlobals->screen_height;
    bufferWidth[0] = pGlobals->screen_width / threadCount;

    for (int i = 1; i < threadCount - 1; i++)
    {
        bufferWidth[i] = bufferWidth[0];
    }

    if (threadCount > 1)
    {
        bufferWidth[threadCount - 1] = pGlobals->screen_width
                - (threadCount - 1) * bufferWidth[0];
    }

    int offset = 0;
    for (int i = 0; i < threadCount; i++)
    {
        bufferOffset[i] = offset;
        offset += bufferWidth[i];
    }

    frontBuffer = new QImage*[threadCount];
    swapBuffer = new QImage*[threadCount];
    backBuffer = new QImage*[threadCount];

    for (int i = 0; i < threadCount; i++)
    {
        frontBuffer[i] = new QImage(bufferWidth[i], bufferHeight, QImage::Format_RGB32);
        swapBuffer[i] = new QImage(bufferWidth[i], bufferHeight, QImage::Format_RGB32);
        backBuffer[i] = new QImage(bufferWidth[i], bufferHeight, QImage::Format_RGB32);

        frontBuffer[i]->fill(Qt::black);
    }

    queueMemory = new char[PARALLEL_BUFFER_SIZE];
    frontPainter.Attach(queueMemory, PARALLEL_BUFFER_SIZE);

    swapQueueMemory = new char*[threadCount];
    backQueueMemory = new char*[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        swapQueueMemory[i] = new char[PARALLEL_BUFFER_SIZE];
        backQueueMemory[i] = new char[PARALLEL_BUFFER_SIZE];

        swapPainterWorker[i].Attach(swapQueueMemory[i], PARALLEL_BUFFER_SIZE);
        backPainterWorker[i].Attach(backQueueMemory[i], PARALLEL_BUFFER_SIZE);
    }
}

ParallelRenderer::~ParallelRenderer()
{
    StopWorkers();

    frontPainter.Flush();

    for (int i = 0; i < threadCount; i++)
    {
        backPainterWorker[i].Flush();
        swapPainterWorker[i].Flush();

        delete [] backQueueMemory[i];
        delete [] swapQueueMemory[i];
    }
    delete [] backQueueMemory;
    delete [] swapQueueMemory;

    delete [] queueMemory;

    for (int i = 0; i < threadCount; i++)
    {
        delete backBuffer[i];
        delete swapBuffer[i];
        delete frontBuffer[i];
    }

    delete [] backBuffer;
    delete [] swapBuffer;
    delete [] frontBuffer;
}

QueuedPainter *ParallelRenderer::BeginJob()
{
    frontPainter.Flush();

    return &frontPainter;
}

void ParallelRenderer::EndJob()
{
    frontPainter.Finish();

    for (int i = 0; i < threadCount; i++)
    {
        mutex_queue[i].lock();
        swapPainterWorker[i].Flush();

        if (frontPainter.CanExecute())
        {
            QueuedPainter copyPainter(frontPainter.GetBuffer(), false);
            copyPainter.ExecuteOnDevice(swapPainterWorker[i]);
            swapPainterWorker[i].Finish();
        }

        Q_ASSERT(frontPainter.GetBuffer()->GetWritePosition()
                 == swapPainterWorker[i].GetBuffer()->GetWritePosition());
        mutex_queue[i].unlock();
    }

    if (threadPool == nullptr)
        StartWorkers();
}

void ParallelRenderer::SwapBuffers()
{
    for (int i = 0; i < threadCount; i++)
    {
        mutex_buffer[i].lock();

        if (swapBufferDirty[i])
        {
            qSwap(swapBuffer[i], frontBuffer[i]);
            swapBufferDirty[i] = false;
        }

        mutex_buffer[i].unlock();
    }

    frameTimeSwap = CalcFrameTime(lastTimeSwap);
}

void ParallelRenderer::OnWorkerFinished(int worker)
{
    frameTimeRender[worker] = CalcFrameTime(lastTimeRender[worker]);
}

void ParallelRenderer::StartWorkers()
{
    Q_ASSERT(threadPool == nullptr);

    timer.start();

    threadPool = new QThreadPool();
    threadPool->setMaxThreadCount(threadCount);

    for (int i = 0; i < threadCount; i++)
    {
        threadPool->start(new ParallelRenderWorker(i, this));
    }
}

void ParallelRenderer::StopWorkers()
{
    stopWorkers = true;

    delete threadPool;
    threadPool = nullptr;
}
