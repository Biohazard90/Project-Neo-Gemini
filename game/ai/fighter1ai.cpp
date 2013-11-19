
#include "gamebase.h"
#include "aibase.h"

#define FIGHTER_1_X_SPEED -180.0f
#define FIGHTER_1_Y_SPEED_MIN 120.0f
#define FIGHTER_1_Y_SPEED_MAX 140.0f

class Fighter1AI : public AbstractAI
{
    float startTime;
    bool movingDown;
    int salvoCount;
    float yspeed;

public:
    virtual void Spawn()
    {
        yspeed = qlerp(qfrand(), FIGHTER_1_Y_SPEED_MIN, FIGHTER_1_Y_SPEED_MAX);
        startTime = GetGameContext()->GetGameTime() + qfrand() * 1.0f;
        salvoCount = 0;
        movingDown = qbrand();

        float yspeed = movingDown ? -FIGHTER_1_X_SPEED : FIGHTER_1_X_SPEED;
        GetShip()->SetVelocity(Vector2D(FIGHTER_1_X_SPEED, yspeed));
    }

    virtual void OnSimulate(float frametime)
    {
        float originy = GetShip()->GetOrigin().y;
        float sizeyhalf = GetShip()->GetSize().y * 0.5f;

        if (!movingDown
                && originy - sizeyhalf < Camera::GetInstance()->GetWorldMins().y)
        {
            movingDown = true;
            GetShip()->SetVelocity(Vector2D(FIGHTER_1_X_SPEED, yspeed));
        }
        else if (movingDown
                 && originy + sizeyhalf > Camera::GetInstance()->GetWorldMaxs().y)
        {
            movingDown = false;
            GetShip()->SetVelocity(Vector2D(FIGHTER_1_X_SPEED, -yspeed));
        }

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

            salvoCount++;

            if (salvoCount >= 3)
            {
                salvoCount = 0;
                startTime = GetGameContext()->GetGameTime() + 1.5f;
            }
        }
    }

    virtual void OnMove(float frametime)
    {

    }
};

REGISTER_AI(Fighter1AI, fighter1ai);

