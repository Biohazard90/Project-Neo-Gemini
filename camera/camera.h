#ifndef CAMERA_H
#define CAMERA_H

#include "vector2d.h"
#include "globals.h"

#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>

class Camera
{
    static Camera instance;
    Camera();
public:

    static Camera *GetInstance();

    void Reset();

    void SetTranslation(const Vector2D &vec);

    FORCEINLINE void Scale(Vector2D &size)
    {
        size.x = VALUE_PROPORTIONAL(size.x);
        size.y = VALUE_PROPORTIONAL(size.y);
    }

    FORCEINLINE Vector2D GetScreenMins() const
    {
        return Vector2D(0, 0);
    }

    FORCEINLINE Vector2D GetScreenMaxs() const
    {
        return Vector2D(pGlobals->screen_width,
                        pGlobals->screen_height);
    }

    FORCEINLINE Vector2D GetWorldMins() const
    {
        Vector2D v = GetScreenMins();
        ToWorld(v);
        return v;
    }

    FORCEINLINE Vector2D GetWorldMaxs() const
    {
        Vector2D v = GetScreenMaxs();
        ToWorld(v);
        return v;
    }

    void ToScreen(Vector2D &vec) const;
    void ToScreen(QPoint &point) const;
    void ToScreen(QPointF &point) const;
    void ToScreen(QRect &rect) const;
    void ToScreen(QRectF &rect) const;

    void ToWorld(Vector2D &vec) const;

private:
    Vector2D translate;

};

#endif // CAMERA_H
