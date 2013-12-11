#ifndef RESOURCE_H
#define RESOURCE_H

#include <QHash>
#include <QString>

#include "gameutil.h"

#define RESOURCE_PROJECTILE_DEFAULT "default"

class QDomElement;

class Resource
{
    Resource();
    static Resource instance;
public:

    static Resource *GetInstance();

    void Init();

    void OnCheated();
    bool HasEverCheated();

    Resource_Projectile_t GetProjectile(const QString &name) const;
    Resource_Obstacle_t GetObstacle(const QString &name) const;
    Resource_Fighter_t GetFighter(const QString &name) const;

private:

    void LoadProjectiles();
    void LoadObstacles();
    void LoadFighters();

    void ParseDestructible(const QDomElement &node, Resource_Destructible_t &d);

    QString BuildHash();

    QHash<QString, Resource_Projectile_t> projectiles;
    QHash<QString, Resource_Obstacle_t> obstacles;
    QHash<QString, Resource_Fighter_t> fighters;

    QString hash;

    bool hasCheated;
};

#endif // RESOURCE_H
