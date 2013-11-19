#include "precache.h"
#include "base.h"
#include "particlebase.h"

#include <QtXml>
#include <QDirIterator>

Precache Precache::instance;
Precache *Precache::GetInstance()
{
    return &instance;
}

Precache::Precache()
{
}

void Precache::PrecacheAll()
{
    PrecacheAll(OSLocalPath(PATH_SOUND_ROOT), &Precache::PrecacheSound, "wav");
    PrecacheAll(OSLocalPath(PATH_PARTICLE_ROOT), &Precache::PrecacheParticle, "xml");
    PrecacheAll(OSLocalPath(PATH_MATERIAL_ROOT), &Precache::PrecacheMaterial, "xml");
}

void Precache::PrecacheSound(const char *path)
{
    DBGMSG("Precaching sound:" << path);
    AudioManager::GetInstance()->PrecacheSound(path);
}

void Precache::PrecacheParticle(const char *path)
{
    DBGMSG("Precaching particle:" << path);
    ParticlePrecache::GetInstance()->PrecacheParticleSystem(path);
}

void Precache::PrecacheMaterial(const char *path)
{
    DBGMSG("Precaching material:" << path);
    MaterialPrecache::GetInstance()->PrecacheMaterial(path);
}

void Precache::PrecacheAll(const QString &root, PrecacheFunc func, const QString &extension)
{
    PrecacheAll(root, root, func, extension);
}

void Precache::PrecacheAll(const QString &root, const QString &path, PrecacheFunc func, const QString &extension)
{
    QDirIterator itr(path);
    while (itr.hasNext())
    {
        QString nextPath = itr.next();
        QFileInfo info = itr.fileInfo();

        if (nextPath.endsWith("."))
        {
            continue;
        }

        if (info.isDir())
        {
            PrecacheAll(root, nextPath, func, extension);
            continue;
        }

        if (nextPath.endsWith("." + extension))
        {
            nextPath.remove(root);
            std::string str = nextPath.toStdString();
            (this->*func)(str.c_str());
        }
    }
}
