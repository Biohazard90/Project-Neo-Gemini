#ifndef PRECACHE_H
#define PRECACHE_H

#include <QString>

class Precache
{
    static Precache instance;
    Precache();
public:

    static Precache *GetInstance();

    void PrecacheAll();

private:

    typedef void(Precache::*PrecacheFunc)(const char*);

    void PrecacheSound(const char *path);
    void PrecacheParticle(const char *path);
    void PrecacheMaterial(const char *path);

    void PrecacheAll(const QString &root, PrecacheFunc func, const QString &extension);
    void PrecacheAll(const QString &root, const QString &path,
                     PrecacheFunc func, const QString &extension);
};

#endif // PRECACHE_H
