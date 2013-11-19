#include "material.h"

QImage grayscaled(const QImage &image)
{
    QImage img = image;
    int pixels = img.width() * img.height();
    unsigned int *data = (unsigned int *)img.bits();

    for (int i = 0; i < pixels; ++i)
    {
        int val = qGray(data[i]);
        data[i] = qRgba(val, val, val, qAlpha(data[i]));
    }

    return img;
}

QImage tinted(const QImage &image, const QColor &color)
{
    QImage resultImage(image.size(), QImage::Format_ARGB32_Premultiplied);
    resultImage.fill(Qt::black);
    QPainter painter(&resultImage);
    painter.drawImage(0, 0, grayscaled(image));
    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
    painter.fillRect(resultImage.rect(), color);
    painter.end();
    resultImage.setAlphaChannel(image.alphaChannel());
    return resultImage;
}


Material::Material(QImage *texture, float opacity,
                   QPainter::CompositionMode mode, int flags)
{
    this->texture = texture;
    this->mode = mode;
    this->opacity = opacity;
    this->flags = (MaterialFlags_e)flags;

    Init();
}

Material::Material(QImage *texture, float opacity,
                   QPainter::CompositionMode mode, QColor tint, int flags)
{
    this->texture = texture;
    this->mode = mode;
    this->opacity = opacity;
    this->flags = (MaterialFlags_e)flags;

    Init();

    *texture = tinted(*texture, tint);
}

Material::~Material()
{
    delete texture;
}

void Material::Init()
{
    width = texture->width();
    height = texture->height();
    width_half = width / 2;
    height_half = height / 2;
}

void Material::Render(const render_context_t &context) const
{
    float oldOpacity = context.painter->opacity();
    context.painter->save();

    ApplyFlags(context);

    context.painter->setOpacity(oldOpacity * opacity);
    context.painter->drawImage(-width_half, -height_half,
                               *texture);
    context.painter->setOpacity(oldOpacity);
    context.painter->restore();
}

void Material::Render(const Vector2D &origin, const float &rotation, const Vector2D &size,
                      const render_context_t &context) const
{
    context.painter->save();
    float oldOpacity = context.painter->opacity();

    ApplyFlags(context);

    context.painter->setOpacity(oldOpacity * opacity);
    context.painter->translate(origin.x, origin.y);
    context.painter->rotate(rotation);
    context.painter->scale(size.x / this->width,
                           size.y / this->height);
    context.painter->setCompositionMode(mode);
    context.painter->drawImage(-width_half, -height_half,
                               *texture);

    context.painter->restore();
}

void Material::Render(const Vector2D &origin, const float &rotation, const float size,
            const render_context_t &context) const
{
    Vector2D vSize(size, size);
    vSize.x *= width / height;
    Render(origin, rotation, vSize, context);
}

void Material::ApplyFlags(const render_context_t &context) const
{
    if ((flags & MF_NEVER_FILTER) != 0)
    {
        context.painter->setRenderHint(QPainter::Antialiasing, false);
        context.painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    }
}
