
#include "base.h"
#include "vector2d.h"

Vector2D Vector2D::Reflect( Vector2D ray, Vector2D normal )
{
    normal *= ray.DotProduct(normal) * 2.0f;
    ray -= normal;
    return ray;
}

Vector2D Vector2D::AngleDirection( float degree )
{
    double rad = DEG2RAD(degree);
    return Vector2D( cos(rad), sin(rad) );
}
