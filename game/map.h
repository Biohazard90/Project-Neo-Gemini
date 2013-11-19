#ifndef MAP_H
#define MAP_H

#include "entity.h"
#include <QVector>

class QDomElement;
class Layer;
class ScriptEventManager;

class Map : public Entity
{
    DECLARE_CLASS(Map, Entity);
    Map();
public:
    ~Map();

    virtual void Spawn();

    virtual void OnRender(const render_context_t &context);
    virtual void OnRenderForeground(const render_context_t &context);
    virtual void OnSimulate(float frametime);

    void OnEntityRemoved(Entity *entity);

    static Map *CreateMapFromXML(const char *path);

private:
    void LoadFromXML(const char *path);
    void LoadLayers(const QDomElement &root);
    Layer *CreateFillLayer(const QDomElement &root);
    Layer *CreateParticleLayer(const QDomElement &root);

    QVector<Layer*> layersBackground;
    QVector<Layer*> layersForeground;

    ScriptEventManager *scriptEventManager;

    std::string musicIdle;
    std::string musicBoss;
};

#endif // MAP_H
