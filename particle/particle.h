#ifndef PARTICLE_H
#define PARTICLE_H

#include "../util/vector2d.h"

#include <QBrush>

enum ParticleField_e
{
    Field_origin = 0,
    Field_velocity,
    Field_angle,
    Field_angularvelocity,
    Field_size,
    Field_length,
    Field_color,
    Field_alpha,
    Field_lifetime
};

struct particle_data_t
{
    particle_data_t()
    {
        memset(this, 0, sizeof(particle_data_t));

        size = 10;
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;
        lifetime = -1;
        length = 0.1f;
    }

    Vector2D origin;
    Vector2D velocity;

    float angle;
    float angularvelocity;

    float size;
    float length;
    float color[4];
    float colorPen[4];

    float lifetime;
};

struct particle_t
{
    particle_t()
    {
        seed = 0;
    }

    particle_data_t data_initial;
    particle_data_t data_runtime;

    unsigned short seed;

    inline QColor GetColorBrush()
    {
        return QColor(
                255 * data_runtime.color[0],
                255 * data_runtime.color[1],
                255 * data_runtime.color[2],
                255 * data_runtime.color[3]
             );
    }

    inline QColor GetColorPen()
    {
        return QColor(
                255 * data_runtime.colorPen[0],
                255 * data_runtime.colorPen[1],
                255 * data_runtime.colorPen[2],
                255 * data_runtime.colorPen[3]
             );
    }

    inline bool ShouldDie()
    {
        return data_runtime.lifetime == 0.0f;
    }
};

#endif // PARTICLE_H
