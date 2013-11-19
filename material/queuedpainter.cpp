#include "queuedpainter.h"


QueuedPainter::QueuedPainter(char *memory, int size, bool autoCleanup)
    : buffer(memory, size)
    , cleanup(autoCleanup)
{
    Init();
}

QueuedPainter::QueuedPainter( ByteBuffer *externalBuffer, bool autoCleanup )
    : buffer(externalBuffer->GetBase(),
             externalBuffer->GetSizeAllocated(),
             externalBuffer->GetWritePosition())
    , cleanup(autoCleanup)
{
    Init();
}

QueuedPainter::~QueuedPainter()
{
    Q_ASSERT_X(buffer.GetReadPosition() == buffer.GetWritePosition()
               || !cleanup,
               "QueuedPainter", "Possible memory leak");
}

void QueuedPainter::Init()
{
    m_opacity.push(1.0f);
}
