#include "base.h"
#include "camera.h"

Camera Camera::instance;
Camera *Camera::GetInstance()
{
    return &instance;
}

Camera::Camera()
{
    translate.Init();
}

void Camera::Reset()
{
    translate.Init();
}

void Camera::SetTranslation(const Vector2D &vec)
{
    translate = vec;
}

void Camera::ToScreen(Vector2D &vec) const
{
    vec -= translate;
    vec.x = VALUE_PROPORTIONAL(vec.x);
    vec.y = VALUE_PROPORTIONAL(vec.y);
}

void Camera::ToScreen(QPoint &point) const
{
    Vector2D v(point);
    ToScreen(v);
    point = v.AsQPoint();
}

void Camera::ToScreen(QPointF &point) const
{
    Vector2D v(point);
    ToScreen(v);
    point = v.AsQPointF();
}

void Camera::ToScreen(QRect &rect) const
{
    QPoint p0 = rect.topLeft();
    QPoint p1 = rect.bottomRight();

    ToScreen(p0);
    ToScreen(p1);

    rect.setTopLeft(p0);
    rect.setBottomRight(p1);
}

void Camera::ToScreen(QRectF &rect) const
{
    QPointF p0 = rect.topLeft();
    QPointF p1 = rect.bottomRight();

    ToScreen(p0);
    ToScreen(p1);

    rect.setTopLeft(p0);
    rect.setBottomRight(p1);
}

void Camera::ToWorld(Vector2D &vec) const
{
    vec.x = VALUE_NORMALIZE(vec.x);
    vec.y = VALUE_NORMALIZE(vec.y);
    vec += translate;
}
