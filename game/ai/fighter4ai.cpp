
#include "gamebase.h"
#include "aibase.h"

#define FIGHTER_4_X_SPEED -180.0f
#define FIGHTER_4_Y_SPEED_MIN 140.0f
#define FIGHTER_4_Y_SPEED_MAX 180.0f

class Fighter4AI : public AbstractAI
{
    float startTime;
    bool movingDown;
    int salvoCount;
    float yspeed;
    float xspeed ;


public:


    bool IsShipBehindPlayer()
    {
        Player *player = GetGameContext()->GetPlayer();

        if (player == nullptr)
            return false;

        float playerOriginX = GetShip()->GetOrigin().x;


        return playerOriginX < player->GetOrigin().x ;

    }


    virtual void Spawn()
    {
        yspeed = qlerp(qfrand(), FIGHTER_4_Y_SPEED_MIN, FIGHTER_4_Y_SPEED_MAX);
        startTime = GetGameContext()->GetGameTime() + qfrand() * 1.0f;
        salvoCount = 0;
        movingDown = qbrand();


        float yspeed = movingDown ? -FIGHTER_4_X_SPEED : FIGHTER_4_X_SPEED;

        GetShip()->SetVelocity(Vector2D(FIGHTER_4_X_SPEED, yspeed));
    }

    virtual void OnSimulate(float frametime)
    {
        float originy = GetShip()->GetOrigin().y;
        float sizeyhalf = GetShip()->GetSize().y * 0.5f;
        float originx = GetShip()->GetOrigin().x;


        if(originx  > Camera::GetInstance()->GetScreenMaxs().x - 200){

            xspeed = FIGHTER_4_X_SPEED;
        }

        if(IsShipBehindPlayer() ){

            xspeed = 300;

        }else{
            xspeed = FIGHTER_4_X_SPEED;

        }




        if (originy - sizeyhalf < Camera::GetInstance()->GetWorldMins().y || originx  < Camera::GetInstance()->GetWorldMins().x )
        {
            GetShip()->SetVelocity(Vector2D(xspeed, yspeed));
        }
        else if (originy + sizeyhalf > Camera::GetInstance()->GetWorldMaxs().y || originx  < Camera::GetInstance()->GetWorldMins().x )
        {
            GetShip()->SetVelocity(Vector2D(xspeed, -yspeed));
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
            GetShip()->Shoot(Vector2D(0, 0), 20.0f);


            salvoCount++;

            if (salvoCount >= 2)
            {
                salvoCount = 0;
                startTime = GetGameContext()->GetGameTime() + 4.5f;
            }
        }
    }

    virtual void OnMove(float frametime)
    {

    }
};

REGISTER_AI(Fighter4AI, fighter4ai);

