
#include "gamebase.h"
#include "aibase.h"

#define FIGHTER_2_X_SPEED_MIN -160.0f
#define FIGHTER_2_X_SPEED_MAX -140.0f
#define FIGHTER_2_Y_SPEED 160.0f
#define FIGHTER_2_PHASE_SPEED_MIN 2.5f
#define FIGHTER_2_PHASE_SPEED_MAX 3.5f

class Fighter2AI : public AbstractAI
{
    float startTime;
    float phaseOffset;
    float phaseSpeed;
    int salvoCount;
    float xspeed;

public:
    virtual void Spawn()
    {
        xspeed = qlerp(qfrand(), FIGHTER_2_X_SPEED_MIN, FIGHTER_2_X_SPEED_MAX);
        phaseSpeed = qlerp(qfrand(), FIGHTER_2_PHASE_SPEED_MIN, FIGHTER_2_PHASE_SPEED_MAX);
        startTime = GetGameContext()->GetGameTime() + qfrand() * 1.0f;
        salvoCount = 0;
        phaseOffset = qfrand() * C_PI * 2.0f;
    }

    virtual void OnSimulate(float frametime)
    {

        float yspeed = sinf(phaseSpeed * GetGameContext()->GetGameTime() + phaseOffset);

        GetShip()->SetVelocity(Vector2D(xspeed, FIGHTER_2_Y_SPEED * yspeed));


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
                startTime = GetGameContext()->GetGameTime() + 1.3f;
            }
        }
    }

    virtual void OnMove(float frametime)
    {

    }
};

REGISTER_AI(Fighter2AI, fighter2ai);

