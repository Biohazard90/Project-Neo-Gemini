
#include "base.h"
#include "aibase.h"

class SimpleAI : public AbstractAI
{
    float startTime;
    int salvoCount;

public:
    virtual void Spawn()
    {
        GetShip()->SetVelocity(Vector2D(-200, 0));

        startTime = GetGameContext()->GetGameTime() + qfrand() * 1.0f;
        salvoCount = 2;
    }

    virtual void OnSimulate(float frametime)
    {
        if (!IsShipFullyVisible())
        {
            return;
        }

        if (startTime > GetGameContext()->GetGameTime())
        {
            return;
        }

        if (GetShip()->CanShoot())
        {
            GetShip()->Shoot(Vector2D(-1, 0), 20.0f);

            salvoCount--;

            if (salvoCount <= 0)
            {
                salvoCount = 2;
                startTime = GetGameContext()->GetGameTime() + 1.5f;
            }
        }
    }

    virtual void OnMove(float frametime)
    {

    }
};

REGISTER_AI(SimpleAI, simpleai);
