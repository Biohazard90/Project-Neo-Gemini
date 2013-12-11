
#include "resource.h"

#include "base.h"

#include <QtXml>

#define CURRENT_RESOURCE_HASH "17d65ec102a44cfd405a375936acef1c"

Resource Resource::instance;
Resource *Resource::GetInstance()
{
    return &instance;
}

Resource::Resource()
    : hasCheated(false)
{
}

Resource_Projectile_t Resource::GetProjectile(const QString &name) const
{
    if (!projectiles.contains(name))
    {
        if (projectiles.contains(RESOURCE_PROJECTILE_DEFAULT))
        {
            return projectiles[RESOURCE_PROJECTILE_DEFAULT];
        }

        return Resource_Projectile_t();
    }

    return projectiles[name];
}

Resource_Obstacle_t Resource::GetObstacle(const QString &name) const
{
    if (!obstacles.contains(name))
        return Resource_Obstacle_t();

    return obstacles[name];
}

Resource_Fighter_t Resource::GetFighter(const QString &name) const
{
    if (!fighters.contains(name))
        return Resource_Fighter_t();

    return fighters[name];
}

void Resource::Init()
{
    LoadProjectiles();
    LoadObstacles();
    LoadFighters();

    hash = QCryptographicHash::hash(hash.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    //qDebug() << hash;
}

void Resource::OnCheated()
{
    hasCheated = true;
}

bool Resource::HasEverCheated()
{
    return hash != CURRENT_RESOURCE_HASH
            || hasCheated;
}

void Resource::LoadProjectiles()
{
    QDomElement root;
    QString path(PATH_RESOURCE_ROOT);
    path += "projectiles.xml";

    QString content;

    if (!OpenXMLFile(path, root, &content))
    {
        DBGWARNING("!! Failed to load projectile resources");
        return;
    }

    hash += QCryptographicHash::hash(content.toLocal8Bit(), QCryptographicHash::Md5).toHex();

    QDomNodeList nodes = root.childNodes();

    FOREACH_QDOM_NODE(nodes, node)
    {
        QString name = node.firstChildElement("name").text();

        if (name.isEmpty())
            continue;

        Resource_Projectile_t p;

        p.name = name.toStdString();
        p.fire_rate = node.firstChildElement("firerate").text().toFloat();
        p.speed = node.firstChildElement("speed").text().toFloat();
        p.size = node.firstChildElement("size").text().toFloat();
        p.particle_spawn = node.firstChildElement("particle_spawn").text().toStdString();
        p.particle_trail = node.firstChildElement("particle_trail").text().toStdString();
        p.particle_hit = node.firstChildElement("particle_hit").text().toStdString();
        p.sound = node.firstChildElement("sound").text().toStdString();

        projectiles.insert(name, p);
    }
    FOREACH_QDOM_NODE_END;
}

void Resource::LoadObstacles()
{
    QDomElement root;
    QString path(PATH_RESOURCE_ROOT);
    path += "obstacles.xml";

    QString content;

    if (!OpenXMLFile(path, root, &content))
    {
        DBGWARNING("!! Failed to load obstacle resources");
        return;
    }

    hash += QCryptographicHash::hash(content.toLocal8Bit(), QCryptographicHash::Md5).toHex();

    QDomNodeList nodes = root.childNodes();

    FOREACH_QDOM_NODE(nodes, node)
    {
        QString name = node.firstChildElement("name").text();

        if (name.isEmpty())
            continue;

        Resource_Obstacle_t o;

        ParseDestructible(node, o);

        QDomNodeList materialNodes = node.elementsByTagName("material");

        FOREACH_QDOM_NODE(materialNodes, materialNode)
        {
            std::string materialName = materialNode.text().toStdString();

            if (materialName.empty())
                continue;

            Material *material = MaterialPrecache::GetInstance()->GetMaterial(materialName.c_str());

            if (material == nullptr)
                continue;

            o.materials[o.materialCount] = material;
            o.materialCount++;

            if (o.materialCount >= RESOURCE_OBSTACLE_MATERIAL_COUNT)
                break;
        }
        FOREACH_QDOM_NODE_END;

        o.health_min = node.firstChildElement("health_min").text().toInt();
        o.health_max = node.firstChildElement("health_max").text().toInt();
        o.size_min = node.firstChildElement("size_min").text().toFloat();
        o.size_max = node.firstChildElement("size_max").text().toFloat();
        o.angularvelocity_min = node.firstChildElement("angularvelocity_min").text().toFloat();
        o.angularvelocity_max = node.firstChildElement("angularvelocity_max").text().toFloat();
        o.velocity_x_min = node.firstChildElement("velocity_x_min").text().toFloat();
        o.velocity_x_max = node.firstChildElement("velocity_x_max").text().toFloat();
        o.origin_x_offset = node.firstChildElement("origin_x_offset").text().toFloat();

        obstacles.insert(name, o);
    }
    FOREACH_QDOM_NODE_END;
}

void Resource::LoadFighters()
{
    QDomElement root;
    QString path(PATH_RESOURCE_ROOT);
    path += "fighters.xml";

    QString content;

    if (!OpenXMLFile(path, root, &content))
    {
        DBGWARNING("!! Failed to load fighter resources");
        return;
    }

    hash += QCryptographicHash::hash(content.toLocal8Bit(), QCryptographicHash::Md5).toHex();

    QDomNodeList nodes = root.childNodes();

    FOREACH_QDOM_NODE(nodes, node)
    {
        QString name = node.firstChildElement("name").text();

        if (name.isEmpty())
            continue;

        Resource_Fighter_t f;

        ParseDestructible(node, f);

        f.size = node.firstChildElement("size").text().toFloat();
        f.ai = node.firstChildElement("ai").text().toStdString();
        f.projectile = GetProjectile(node.firstChildElement("projectile").text());

        std::string materialName = node.firstChildElement("material").text().toStdString();
        f.material = MaterialPrecache::GetInstance()->GetMaterial(materialName.c_str());

        fighters.insert(name, f);
    }
    FOREACH_QDOM_NODE_END;
}

void Resource::ParseDestructible(const QDomElement &node, Resource_Destructible_t &d)
{
    d.name = node.firstChildElement("name").text().toStdString();
    d.health = node.firstChildElement("health").text().toInt();
    d.score = node.firstChildElement("score").text().toInt();

    d.particle_hit = node.firstChildElement("particle_hit").text().toStdString();
    d.particle_destroy = node.firstChildElement("particle_destroy").text().toStdString();

    FOREACH_QDOM_CHILD(node, sound_hit, soundHit)
    {
        if (d.sound_hit_count >= RESOURCE_OBSTACLE_SOUND_COUNT)
            break;

        d.sound_hit[d.sound_hit_count] = soundHit.text().toStdString();
        d.sound_hit_count++;
    }
    FOREACH_QDOM_CHILD_END;

    FOREACH_QDOM_CHILD(node, sound_destroy, soundDestroy)
    {
        if (d.sound_destroy_count >= RESOURCE_OBSTACLE_SOUND_COUNT)
            break;

        d.sound_destroy[d.sound_destroy_count] = soundDestroy.text().toStdString();
        d.sound_destroy_count++;
    }
    FOREACH_QDOM_CHILD_END
}
