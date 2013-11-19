#ifndef MATERIAL_H
#define MATERIAL_H

#include <QPainter>
#include <QImage>

#include "rendercontext.h"
#include "vector2d.h"
#include "platform.h"


class Material
{
public:
    enum MaterialFlags_e
    {
        MF_NONE = 0,
        MF_NEVER_FILTER = (1 << 0)
    };

    Material(QImage *texture,
             float opacity = 1.0f,
             QPainter::CompositionMode mode = QPainter::CompositionMode_SourceOver,
             int flags = MF_NONE);
    Material(QImage *texture,
             float opacity,
             QPainter::CompositionMode mode,
             QColor tint,
             int flags = MF_NONE);
    ~Material();

    void Render(const render_context_t &context) const;
    void Render(const Vector2D &origin,
                const float &rotation,
                const Vector2D &size,
                const render_context_t &context) const;
    void Render(const Vector2D &origin,
                const float &rotation,
                const float size,
                const render_context_t &context) const;

    FORCEINLINE const QImage *GetTexture() const
    {
        return texture;
    }

    FORCEINLINE const QPainter::CompositionMode &GetMode() const
    {
        return mode;
    }

    FORCEINLINE const float &GetOpacity() const
    {
        return opacity;
    }

    FORCEINLINE const int &GetWidth() const
    {
        return width;
    }

    FORCEINLINE const int &GetHeight() const
    {
        return height;
    }

    FORCEINLINE const int &GetWidthHalf() const
    {
        return width_half;
    }

    FORCEINLINE const int &GetHeightHalf() const
    {
        return height_half;
    }

private:
    void Init();
    void ApplyFlags(const render_context_t &context) const;

    QImage *texture;
    QPainter::CompositionMode mode;
    float opacity;

    int width, height;
    int width_half, height_half;
    int flags;
};

#endif // MATERIAL_H
