#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <QMetaType>

#include "platform.h"

#include <QDebug>

class ByteBuffer
{
    ByteBuffer(const ByteBuffer &other);

public:
    ByteBuffer(char *destBuffer = nullptr, int destBufferSize = 0, int writePosition = 0);
    ~ByteBuffer();

    FORCEINLINE void Swap(ByteBuffer &other)
    {
        qSwap(size, other.size);
        qSwap(growsize, other.growsize);
        qSwap(position, other.position);
        qSwap(position_read, other.position_read);
        qSwap(buffer, other.buffer);
        qSwap(external, other.external);
    }

    FORCEINLINE void Attach(char *destBuffer, int destBufferSize)
    {
        external = true;

        size = destBufferSize;
        buffer = destBuffer;

        position = 0;
        position_read = 0;
        growsize = 32;
    }

    FORCEINLINE void Detach()
    {
        Q_ASSERT(external);

        buffer = nullptr;
        size = 0;
        position = 0;
        position_read = 0;
    }

    FORCEINLINE void Reset()
    {
        position = 0;
        position_read = 0;
    }

    FORCEINLINE int GetSizeAllocated() const
    {
        return size;
    }

    FORCEINLINE int GetReadPosition() const
    {
        return position_read;
    }

    FORCEINLINE int GetWritePosition() const
    {
        return position;
    }

    FORCEINLINE char *GetBase() const
    {
        return buffer;
    }

    FORCEINLINE void Write(const void *data, const int &size)
    {
        GrowOnDemand(size);

        Q_ASSERT(size <= (this->size + size));

        memcpy(&buffer[position], data, size);
        position += size;
    }

    FORCEINLINE void WriteBool(const bool &v)
    {
        Write(&v, sizeof(bool));
    }

    FORCEINLINE void WriteChar(const char &v)
    {
        Write(&v, sizeof(char));
    }

    FORCEINLINE void WriteUnsignedChar(const unsigned char &v)
    {
        Write(&v, sizeof(unsigned char));
    }

    FORCEINLINE void WriteInt(const int &v)
    {
        Write(&v, sizeof(int));
    }

    FORCEINLINE void WriteFloat(const float &v)
    {
        Write(&v, sizeof(float));
    }

    FORCEINLINE void WriteDouble(const double &v)
    {
        Write(&v, sizeof(double));
    }

    FORCEINLINE void WriteQReal(const qreal &v)
    {
        Write(&v, sizeof(qreal));
    }

    FORCEINLINE void WritePtr(const void *v)
    {
        Write(&v, sizeof(void*));
    }

    FORCEINLINE void Read(const int &size, void *data)
    {
        Q_ASSERT((position_read + size) <= position);

        memcpy(data, &buffer[position_read], size);
        position_read += size;
    }

    template< class T >
    FORCEINLINE T Read(const int &size)
    {
        Q_ASSERT((position_read + size) <= position);

        T data;
        memcpy(&data, &buffer[position_read], size);
        position_read += size;
        return data;
    }

    FORCEINLINE void Flush(const int &size)
    {
        position_read += size;
    }

    FORCEINLINE void ReadBool(bool *v)
    {
        Read(sizeof(bool), v);
    }

    FORCEINLINE bool ReadBool()
    {
        return Read< bool >(sizeof(bool));
    }

    FORCEINLINE void FlushBool()
    {
        Flush( sizeof(bool) );
    }

    FORCEINLINE void ReadChar(char *v)
    {
        Read(sizeof(char), v);
    }

    FORCEINLINE char ReadChar()
    {
        return Read< char >(sizeof(char));
    }

    FORCEINLINE void FlushChar()
    {
        Flush( sizeof(char) );
    }

    FORCEINLINE void ReadUnsignedChar(unsigned char *v)
    {
        Read(sizeof(unsigned char), v);
    }

    FORCEINLINE unsigned char ReadUnsignedChar()
    {
        return Read< unsigned char >(sizeof(unsigned char));
    }

    FORCEINLINE void FlushUnsignedChar()
    {
        Flush( sizeof(unsigned char) );
    }

    FORCEINLINE void ReadInt(int *v)
    {
        Read(sizeof(int), v);
    }

    FORCEINLINE int ReadInt()
    {
        return Read< int >(sizeof(int));
    }

    FORCEINLINE void FlushInt()
    {
        Flush( sizeof(int) );
    }

    FORCEINLINE void ReadFloat(float *v)
    {
        Read(sizeof(float), v);
    }

    FORCEINLINE float ReadFloat()
    {
        return Read< float >(sizeof(float));
    }

    FORCEINLINE void FlushFloat()
    {
        Flush( sizeof(float) );
    }

    FORCEINLINE void ReadDouble(double *v)
    {
        Read(sizeof(double), v);
    }

    FORCEINLINE double ReadDouble()
    {
        return Read< double >(sizeof(double));
    }

    FORCEINLINE void FlushDouble()
    {
        Flush( sizeof(double) );
    }

    FORCEINLINE void ReadQReal(qreal *v)
    {
        Read(sizeof(qreal), v);
    }

    FORCEINLINE qreal ReadQReal()
    {
        return Read< qreal >(sizeof(qreal));
    }

    FORCEINLINE void FlushQReal()
    {
        Flush( sizeof(qreal) );
    }

    template< class T >
    FORCEINLINE void ReadPtr(T *v)
    {
        Read(sizeof(T), v);
    }

    template< class T >
    FORCEINLINE T ReadPtr()
    {
        return Read< T >(sizeof(T));
    }

    FORCEINLINE void FlushPtr()
    {
        Flush( sizeof(void*) );
    }


private:

    FORCEINLINE void GrowOnDemand(const int &addBytes)
    {
        if (external)
            return;

        Q_ASSERT(growsize > 0);

        if (position + addBytes > size)
        {
            int grow = (position + addBytes) / growsize + 1;
            Grow(grow * growsize);
        }

        Q_ASSERT(size >= (position + addBytes));
    }

    FORCEINLINE void Grow(const int &addBytes)
    {
        Q_ASSERT(addBytes > 0);
        Q_ASSERT(!external);

        int newSize = size + addBytes;
        char *newBuffer = new char[newSize];
        memmove(newBuffer, buffer, sizeof(char) * position);

        delete [] buffer;
        buffer = newBuffer;
        size = newSize;
    }

    int growsize;
    int size;
    int position;
    int position_read;

    char *buffer;
    bool external;
};

#endif // BYTEBUFFER_H
