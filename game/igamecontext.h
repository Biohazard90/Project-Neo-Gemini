#ifndef IGAMECONTEXT_H
#define IGAMECONTEXT_H

#include "vector2d.h"

class Entity;
class ParticleRoot;
class Player;

class IGameContext
{
public:
    virtual ~IGameContext(){}

    virtual float GetGameTime() = 0;

    virtual Entity *CreateEntity(const QString &scriptName) = 0;
    virtual Entity *CreateEntityNoSpawn(const QString &scriptName) = 0;
    virtual void SpawnEntity(Entity *entity) = 0;
    virtual void RemoveEntity(Entity *entity) = 0;

    virtual ParticleRoot *GetParticleRoot() = 0;

    virtual Player *GetPlayer() = 0;

    virtual void EndMap() = 0;

    virtual void ShowWarningText() = 0;
    virtual void ShowCutscene(QString portraitLeft, QString portraitRight, QString titel, QString message) = 0;

};

#endif // IGAMECONTEXT_H
