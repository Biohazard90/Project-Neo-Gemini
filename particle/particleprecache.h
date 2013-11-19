#ifndef PARTICLEPRECACHE_H
#define PARTICLEPRECACHE_H

#include "particlesystem.h"

class IParticleFunction;
class QDomElement;

class ParticlePrecache
{
    static ParticlePrecache instance;
    ParticlePrecache();
    ~ParticlePrecache();
public:

    static ParticlePrecache *GetInstance();

    void PrecacheParticleSystem(const char *path);

    ParticleSystem *CreateParticleSystem(const char *name);

private:

    ParticleSystem *ParseParticleSystem(const char *path);

    template< class T >
    void ParseSiblingFunctions(QDomElement *root, QVector<T> &out);

    ParticleSystem *CreateSystemCopy(ParticleSystem *src);

    template< class T >
    void CreateFunctionCopy(QVector<T> &source, QVector<T> &dest);

    QHash<QString, ParticleSystem*> systems;
};

#endif // PARTICLEPRECACHE_H
