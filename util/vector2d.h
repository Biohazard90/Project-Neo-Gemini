#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QMetaType>
#include <QPoint>
#include <QPointF>

#include "macros.h"

class Vector2D
{
public:
    FORCEINLINE Vector2D();
    FORCEINLINE Vector2D(float x, float y);
    FORCEINLINE Vector2D(const QPoint &point);
    FORCEINLINE Vector2D(const QPointF &point);

    FORCEINLINE float Length() const;
    FORCEINLINE float LengthSqr() const;
    FORCEINLINE float Normalize();
    FORCEINLINE float DotProduct(const Vector2D &other) const;
    FORCEINLINE void Rotate(float degrees);
    FORCEINLINE void Rotate(const Vector2D &forward);
    FORCEINLINE Vector2D GetPerpendicular() const;
    FORCEINLINE float Angle() const;

    static Vector2D Reflect( Vector2D ray, Vector2D normal );
    static Vector2D AngleDirection( float degree );

    FORCEINLINE void Init(float x = 0, float y = 0);

    FORCEINLINE QPoint AsQPoint() const;
    FORCEINLINE QPointF AsQPointF() const;

    FORCEINLINE bool IsZero() const;
    FORCEINLINE bool AnyZero() const;

    FORCEINLINE Vector2D &operator+=(const Vector2D &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    FORCEINLINE Vector2D &operator-=(const Vector2D &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    FORCEINLINE Vector2D &operator/=(const Vector2D &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }

    FORCEINLINE Vector2D &operator*=(const Vector2D &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }

    FORCEINLINE const Vector2D operator+(const Vector2D &other) const
    {
        Vector2D v(*this);
        v.x += other.x;
        v.y += other.y;
        return v;
    }

    FORCEINLINE const Vector2D operator-(const Vector2D &other) const
    {
        Vector2D v(*this);
        v.x -= other.x;
        v.y -= other.y;
        return v;
    }

    FORCEINLINE const Vector2D operator/(const Vector2D &other) const
    {
        Vector2D v(*this);
        v.x /= other.x;
        v.y /= other.y;
        return v;
    }

    FORCEINLINE const Vector2D operator*(const Vector2D &other) const
    {
        Vector2D v(*this);
        v.x *= other.x;
        v.y *= other.y;
        return v;
    }

    FORCEINLINE Vector2D &operator+=(const float &f)
    {
        this->x += f;
        this->y += f;
        return *this;
    }

    FORCEINLINE Vector2D &operator-=(const float &f)
    {
        this->x -= f;
        this->y -= f;
        return *this;
    }

    FORCEINLINE Vector2D &operator/=(const float &f)
    {
        this->x /= f;
        this->y /= f;
        return *this;
    }

    FORCEINLINE Vector2D &operator*=(const float &f)
    {
        this->x *= f;
        this->y *= f;
        return *this;
    }

    FORCEINLINE const Vector2D operator+(const float &f) const
    {
        Vector2D v(*this);
        v.x += f;
        v.y += f;
        return v;
    }

    FORCEINLINE const Vector2D operator-(const float &f) const
    {
        Vector2D v(*this);
        v.x -= f;
        v.y -= f;
        return v;
    }

    FORCEINLINE const Vector2D operator/(const float &f) const
    {
        Vector2D v(*this);
        v.x /= f;
        v.y /= f;
        return v;
    }

    FORCEINLINE const Vector2D operator*(const float &f) const
    {
        Vector2D v(*this);
        v.x *= f;
        v.y *= f;
        return v;
    }

    FORCEINLINE const Vector2D operator-() const
    {
        Vector2D v(-x, -y);
        return v;
    }

    FORCEINLINE float operator[](const int &i) const
    {
        return (i==0) ? x : y;
    }

    FORCEINLINE float &operator[](const int &i)
    {
        return (i==0) ? x : y;
    }

    /*
    FORCEINLINE operator QPoint() const
    {
        return QPoint(x, y);
    }

    FORCEINLINE operator QPointF() const
    {
        return QPointF(x, y);
    }
    */

    float x, y;
};

Vector2D::Vector2D()
{
    x = y = 0;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D(const QPoint &point)
{
    x = point.x();
    y = point.y();
}

Vector2D::Vector2D(const QPointF &point)
{
    x = point.x();
    y = point.y();
}

void Vector2D::Init(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vector2D::Length() const
{
    return sqrt(LengthSqr());
}


float Vector2D::LengthSqr() const
{
    return x * x + y * y;
}

float Vector2D::Normalize()
{
    float f = Length();
    if (f == 0.0f)
    {
        return 0.0f;
    }
    x /= f;
    y /= f;
    return f;
}

float Vector2D::DotProduct(const Vector2D &other) const
{
    return x * other.x + y * other.y;
}

void Vector2D::Rotate(float degrees)
{
    float r = DEG2RAD( degrees );
    float c = cos( r );
    float s = sin( r );
    float x0 = x;
    x = c * x - s * y;
    y = s * x0 + c * y;
}

Vector2D Vector2D::GetPerpendicular() const
{
    return Vector2D( y, -x );
}

QPoint Vector2D::AsQPoint() const
{
    return QPoint(x, y);
}

QPointF Vector2D::AsQPointF() const
{
    return QPointF(x, y);
}

bool Vector2D::IsZero() const
{
    return x == 0.0f && y == 0.0f;
}

bool Vector2D::AnyZero() const
{
    return x == 0.0f || y == 0.0f;
}

void Vector2D::Rotate(const Vector2D &forward)
{
    Q_ASSERT(!forward.IsZero());

    Vector2D perp = forward.GetPerpendicular();
    Vector2D tmp(*this);

    x = forward.DotProduct(tmp);
    y = perp.DotProduct(tmp);
}

float Vector2D::Angle() const
{
    return RAD2DEG(atan2(y, x));
}

static const Vector2D vec2_origin( 0, 0 );
static const Vector2D vec2_forward( 1, 0 );

static const QPointF pointf_origin( 0, 0 );
static const QPoint point_origin( 0, 0 );

#endif // VECTOR2D_H
