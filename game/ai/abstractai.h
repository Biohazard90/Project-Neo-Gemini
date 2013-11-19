#ifndef ABSTRACTAI_H
#define ABSTRACTAI_H

#include "iai.h"

class Ship;

class AbstractAI : public IAI
{
public:
    AbstractAI();

    virtual void Init(Ship *ship, IGameContext *gameContext);

    virtual void OnRemove(){}

protected:
    Ship *GetShip() const;
    IGameContext *GetGameContext() const;

    bool IsShipFullyVisible() const;

private:
    Ship *ship;
    IGameContext *gameContext;

};

#endif // ABSTRACTFLIGHTAI_H
