#include "bytebuffer.h"


ByteBuffer::ByteBuffer(char *destBuffer, int destBufferSize, int writePosition)
{
    position_read = 0;
    position = writePosition;

    if (destBuffer == NULL)
    {
        external = false;
        growsize = 32;

        size = 1;
        buffer = new char[size];
    }
    else
    {
        external = true;
        growsize = 0;

        size = destBufferSize;
        buffer = destBuffer;
    }
}

ByteBuffer::~ByteBuffer()
{
    if (!external)
        delete [] buffer;
}
