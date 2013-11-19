#ifndef IPARTICLEFUNCTION_H
#define IPARTICLEFUNCTION_H

#include "platform_warnings.h"

class QDomElement;

#define PARTICLE_PARAMS_BEGIN \
    { \
        if (false) {

#define PARTICLE_PARAMS_END \
        } else \
            return false; \
    }

#define PARTICLE_PARAMS_BASECLASS(baseclass) \
    if ( baseclass::ParseFromXML(tag, data, source) ) \
        return true

#define PARTICLE_PARAM_FLOAT( x ) \
        } else if ( qstricmp(#x, tag) == 0 ) \
                    { x = atof(data); return true

#define PARTICLE_PARAM_INT( x ) \
        } else if ( qstricmp(#x, tag) == 0 ) \
                    { x = atoi(data); return true

#define PARTICLE_PARAM_BOOL( x ) \
        } else if ( qstricmp(#x, tag) == 0 ) \
                    { x = atoi(data) != 0 || qstricmp("true", data) == 0; return true

#define PARTICLE_PARAM_STRING( x ) \
        } else if ( qstricmp(#x, tag) == 0 ) \
                    { x = data; return true

#define PARTICLE_PARAM_CUSTOM( x ) \
    } else if ( qstricmp(#x, tag) == 0 ) \
        {

class IParticleFunction
{
public:
    virtual ~IParticleFunction(){}

    virtual IParticleFunction *Copy() = 0;

    virtual bool ParseFromXML(const char *tag, const char *data, QDomElement *source) = 0;
};

#endif // IPARTICLEFUNCTION_H
