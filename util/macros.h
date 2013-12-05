#ifndef MACROS_H
#define MACROS_H

#include "platform.h"

#define C_PI 3.14159265
#define C_PI_F 3.14159265f

#define GAME_SPEED_SCALE 1.0f
#define PLAYER_DEFAULT_LIVE_COUNT 1

#define DBG_ENABLE 1

#if DBG_ENABLE == 1
#define DBGMSG(x) ((void)0)
#define DBGWARNING(x) \
    qWarning() << x
#elif DBG_ENABLE == 2
#define DBGMSG(x) \
    qDebug() << x
#define DBGWARNING(x) \
    qWarning() << x
#else
#define DBGMSG(x) ((void)0)
#define DBGWARNING(x) ((void)0)
#endif

#define QARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

#define DECLARE_CLASS(name, base) \
    typedef name ThisClass; \
    typedef base BaseClass

#define DECLARE_ENTITY_CLASS(name, base) \
    typedef name ThisClass; \
    typedef base BaseClass; \
    public: \
    virtual const char *GetEntityClassName() { return constClassName; } \
    private: \
    static const char *constClassName

#define DECLARE_ENTITY_CLASS_NOBASE(name) \
    typedef name ThisClass; \
    static const char *constClassName

#define GET_QHASH(hash, key) \
    ((hash).contains((key)) ? (hash)[(key)] : nullptr)

#define DESTROY_QLIST(list) \
    { while(!list.isEmpty()) { delete list.takeFirst(); } }

#define DESTROY_QVECTOR(vector) \
    { \
        for (int __i = 0; __i < vector.size(); __i++) \
            delete vector.at(__i); \
        vector.clear(); \
    }

#define DESTROY_QHASH(keyType, valueType, hash) \
    { \
        QHashIterator<keyType,valueType> itr(hash); \
        while (itr.hasNext()) \
        { \
            delete itr.next().value(); \
        } \
    }

#define FOREACH_QDOM_NODE(list, node) \
    for ( int i##list##node = 0; \
          i##list##node < list.length(); \
          i##list##node ++ ) \
    { \
        QDomElement node = list.at(i##list##node).toElement();
#define FOREACH_QDOM_NODE_END }

#define FOREACH_QDOM_CHILD(root, name, node) \
    for ( QDomElement node = root.firstChildElement(#name); \
            !node.isNull(); \
            node = node.nextSiblingElement(#name) ) \
    {
#define FOREACH_QDOM_CHILD_END }

#define FOREACH_QLIST(list, type, element) \
    { \
        QListIterator<type> itr##element(list); \
        while(itr##element.hasNext()) \
        { \
            type element = itr##element.next();
#define FOREACH_QLIST_END } }

#define FOREACH_QVECTOR(vector, type, element) \
    { \
        for (int i##element = 0; i##element < vector.size(); i##element++) \
        { \
            type element = vector.at(i##element);
#define FOREACH_QVECTOR_END } }

// immutable
#define FOREACH_QVECTOR_FAST(vector, type, element) \
    { \
        int __c##element = vector.size(); \
        type* itr##element = vector.data(); \
        for (int i##element = 0; i##element < __c##element; i##element++) \
        { \
            type element = itr##element[i##element];
#define FOREACH_QVECTOR_FAST_CONST(vector, type, element) \
    { \
        int __c##element = vector.size(); \
        type const* itr##element = vector.data(); \
        for (int i##element = 0; i##element < __c##element; i##element++) \
        { \
            type element = itr##element[i##element];
#define FOREACH_QVECTOR_FAST_END } }


#define DEG2RAD(x) (C_PI/180.0f*((float)x))
#define RAD2DEG(x) (180.0f/C_PI*((float)x))

#define DEFAULT_FRAMESPEED_MS 15
#define DEFAULT_FRAMESPEED_SECF (DEFAULT_FRAMESPEED_MS/1000.0f)

#define CLAMP(x,min,max) \
    (((x)<(min))?(min):((x)>(max))?(max):(x))

#define QSEL(v, a, b) \
    ((v)>=0?(a):(b))

#define MIN(a, b) \
    (((a)<(b))?(a):(b))

#define MAX(a, b) \
    (((a)<(b))?(b):(a))

#define SATURATE(x) \
    CLAMP(x, 0.0f, 1.0f)

#define NORMAL_SCREEN_HEIGHT (480)
#define NORMAL_SCREEN_WIDTH (640)

#define VALUE_PROPORTIONAL(x) ((x)*(pGlobals->screen_height/(float)NORMAL_SCREEN_HEIGHT))
#define VALUE_NORMALIZE(x) ((x)*(NORMAL_SCREEN_HEIGHT/(float)pGlobals->screen_height))

#define VALUE_PROPORTIONAL_X(x) ((x)*(pGlobals->screen_width/(float)NORMAL_SCREEN_WIDTH))
#define VALUE_NORMALIZE_X(x) ((x)*(NORMAL_SCREEN_WIDTH/(float)pGlobals->screen_width))

#define QREMAPVAL( val, a, b, c, d) \
    ( \
        ( a == b ) ? \
           QSEL( val - b , d , c ) \
        : ( c + (d - c) * (val - a) / (b - a) ) \
    )

#define QREMAPVALCLAMPED( val, a, b, c, d ) \
    ( \
         ( a == b ) ? \
            QSEL( val - b , d , c ) \
        : (c + ( d - c ) * CLAMP( (( val - a ) / ( b - a )), 0.0, 1.0 )) \
    )

#define VECTOR_PROPORTIONAL_SCREENSPACE(v) \
    v.y = VALUE_PROPORTIONAL(v.y); \
    v.x = VALUE_PROPORTIONAL(v.x); \
    v.x += ( pGlobals->screen_width - VALUE_PROPORTIONAL((float)NORMAL_SCREEN_WIDTH) ) / 2.0f;


#endif // MACROS_H
