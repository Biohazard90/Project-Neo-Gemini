#include "aibase.h"
#include "base.h"

AbstractAI::AbstractAI()
    : ship(nullptr)
    , gameContext(nullptr)
{
}

void AbstractAI::Init(Ship *ship, IGameContext *gameContext)
{
    this->ship = ship;
    this->gameContext = gameContext;
}

Ship *AbstractAI::GetShip() const
{
    return ship;
}

IGameContext *AbstractAI::GetGameContext() const
{
    return gameContext;
}

bool AbstractAI::IsShipFullyVisible() const
{
    return ship->GetOrigin().x + ship->GetSize().x
            < Camera::GetInstance()->GetWorldMaxs().x;
}
