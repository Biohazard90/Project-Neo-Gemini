#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef PLATFORM_WINDOWS
#define FORCEINLINE __forceinline

#elif defined(PLATFORM_UNIX) || defined(PLATFORM_OSX)
// __attribute__((always_inline))
#define FORCEINLINE inline

#include "float.h"
#include "math.h"
#else
#error implement_me
#endif

#endif // PLATFORM_H
