#ifndef UTIL_H
#define UTIL_H


#include "platform.h"
#include "macros.h"
#include "vector2d.h"
#include "particle.h"

#include <QPainter>

class QDomDocument;
class QDomElement;

#define PATH_MATERIAL_ROOT "content/materials/"
#define PATH_PARTICLE_ROOT "content/particles/"
#define PATH_SOUND_ROOT "content/sounds/"
#define PATH_MAP_ROOT "content/maps/"
#define PATH_RESOURCE_ROOT "content/resources/"
#define PATH_SCORE_ROOT "content/score/"

extern void CenterOnScreen(QWidget *pWidget);

FORCEINLINE QString OSLocalPath(const QString &path)
{
#ifdef PLATFORM_OSX
    return "../../../" + path;
#else
    return path;
#endif
}

extern QString OSUserName();

template< class T >
FORCEINLINE T sqr(const T &t)
{
    return t * t;
}


template< class T >
FORCEINLINE const T qsign(const T &value)
{
    return (value < 0.0) ? -1.0 : (value > 0.0) ? 1.0 : 0.0;
}

template< class T >
FORCEINLINE const T qapproachlinear(const T &target, const T &current, const T &speed)
{
    T d = target - current;

    if (d == 0.0f)
        return target;

    if (speed > qAbs(d))
        return target;
    else
        return current + qsign(d) * speed;
}

template< class T >
FORCEINLINE float qremapval( T val, const T &a, const T &b, const T &c, const T &d)
{
    if ( a == b )
        return QSEL( val - b , d , c );
    return c + (d - c) * (val - a) / (b - a);
}

template <class T >
FORCEINLINE float qremapvalclamped( T val, const T &a, const T &b, const T &c, const T &d )
{
    if ( a == b )
        return QSEL( val - b , d , c );
    float f = ( val - a ) / ( b - a );
    f = CLAMP( f, 0.0f, 1.0f );
    return c + ( d - c ) * f;
}

template< class T >
FORCEINLINE T qlerp( float perc, T const &a, T const &b )
{
    return a + ( b - a ) * perc;
}

template< class T >
FORCEINLINE T clamp_angle_positive( T const &angle )
{
    T v = fmod(angle, 360.0);
    if (v < 0)
        v += 360.0;
    return v;
}

template< class T >
FORCEINLINE T clamp_angle( T const &angle )
{
    T v = fmod(angle + 180.0f, 360.0);
    if (v < 0)
        v += 360.0;
    return v - 180.0f;
}

FORCEINLINE bool IntersectBounds(const Vector2D &amin, const Vector2D &amax,
                           const Vector2D &bmin, const Vector2D &bmax)
{
    if ( amax.x < bmin.x
         || bmax.x < amin.x
         || amax.y < bmin.y
         || bmax.y < amin.y )
    {
        return false;
    }

    return true;
}

FORCEINLINE bool IntersectSizeBounds(const Vector2D &aorigin, const Vector2D &asize,
                                    const Vector2D &bmin, const Vector2D &bmax)
{
    Vector2D amin = aorigin - asize * 0.5f;
    Vector2D amax = aorigin + asize * 0.5f;
    return IntersectBounds(amin, amax, bmin, bmax);
}

FORCEINLINE bool IntersectSize(const Vector2D &aorigin, const Vector2D &asize,
                                    const Vector2D &borigin, const Vector2D &bsize)
{
    Vector2D amin = aorigin - asize * 0.5f;
    Vector2D amax = aorigin + asize * 0.5f;
    Vector2D bmin = borigin - bsize * 0.5f;
    Vector2D bmax = borigin + bsize * 0.5f;
    return IntersectBounds(amin, amax, bmin, bmax);
}


extern QPainter::CompositionMode StringToCompositionMode(const QString &string);
extern ParticleField_e StringToParticleField(const QString &string);

extern bool OpenXMLFile(const QString &path, QDomDocument &dest);
extern bool OpenXMLFile(const QString &path, QDomElement  &root);
extern QDomElement XMLFirstChildElementNoRecursion(const QDomElement &root, const QString &name);

extern QColor XMLParseColor(const QDomElement &node);
extern Vector2D XMLParseVector(const QDomElement &node);
extern QPoint XMLParsePoint(const QDomElement &node);
extern QPointF XMLParsePointF(const QDomElement &node);
extern bool XMLParseBoolean(const QDomElement &node, bool defaultValue = false);
extern int XMLParseInt(const QDomElement &node, int defaultValue = 0);
extern float XMLParseFloat(const QDomElement &node, float defaultValue = 0);

inline float qfrand()
{
    return qrand() / (float)RAND_MAX;
}

inline int qrand(int max)
{
    int v = qlerp(qfrand(), 0.5f, (float)(max + 0.5f));
    return CLAMP(v, 0, max);
}


inline bool qbrand()
{
    return qrand() > (RAND_MAX / 2);
}

inline char *_strcreatecpy(const char *str)
{
    size_t len = strlen(str);
    char *ret = new char[len + 1];
    strcpy_s(ret, len + 1, str);
    return ret;
}

inline bool _streq(const char *str1, const char *str2)
{
    return _stricmp(str1, str2) == 0;
}

#endif // UTIL_H
