
#include "gamebase.h"
#include "aibase.h"

#define FIGHTER_3_X_SPEED -170.0f
#define FIGHTER_3_Y_SPEED_MIN 140.0f
#define FIGHTER_3_Y_SPEED_MAX 200.0f

class Fighter3AI : public AbstractAI
{
    float startTime;
    float preventDirectionTriggerTime;
    float yspeed;
    float waitTime;
    float chargeTime;
    bool playerLastDirectionSign;

    QPointer<ParticleSystem> particlesCharge;
    QPointer<ParticleSystem> particlesFire;

public:
    virtual void Spawn()
    {
        yspeed = qlerp(qfrand(), FIGHTER_3_Y_SPEED_MIN, FIGHTER_3_Y_SPEED_MAX);
        startTime = GetGameContext()->GetGameTime() + qfrand() * 1.0f;
        waitTime = 0.0f;
        chargeTime = 0.0f;
        preventDirectionTriggerTime = 0.0f;
        playerLastDirectionSign = GetPlayerDirectionSign();
    }

    bool GetPlayerDirectionSign()
    {
        Player *player = GetGameContext()->GetPlayer();

        if (player == nullptr)
            return false;

        float playerOriginY = GetShip()->GetOrigin().y;

        if (!player->IsAlive())
            playerOriginY = 0;

        return playerOriginY > player->GetOrigin().y;
    }

    virtual void OnRemove()
    {
        AbstractAI::OnRemove();

        if (!particlesCharge.isNull())
            particlesCharge->StopEmissionAndParticles();

        if (!particlesFire.isNull())
            particlesFire->StopEmissionAndParticles();
    }

    virtual void OnSimulate(float frametime)
    {
        Player *player = GetGameContext()->GetPlayer();
        float desiredYSpeed = 0.0f;
        const float gameTime = GetGameContext()->GetGameTime();
        const bool playerDirectionSign = GetPlayerDirectionSign();

        if (player != nullptr
                && player->IsAlive()
                && waitTime < gameTime)
        {
            const float yPlayer = player->GetOrigin().y;
            const float ySelf = GetShip()->GetOrigin().y;
            const float ySign = qsign(yPlayer - ySelf);

            desiredYSpeed = ySign * yspeed;
        }

        float yLastSpeed = GetShip()->GetVelocity().y;
        yLastSpeed = qapproachlinear(desiredYSpeed, yLastSpeed, frametime * 400.0f);

        GetShip()->SetVelocity(Vector2D(FIGHTER_3_X_SPEED, yLastSpeed));

        //TODO: move laser beam animation + behavior into LaserBeam class
        if (chargeTime > 0.0f && chargeTime < gameTime)
        {
            chargeTime = 0.0f;

            LaserBeam *beam = (LaserBeam*)GetGameContext()->CreateEntityNoSpawn("laserbeam");
            if (beam != nullptr)
            {
                beam->Init(GetShip(), Vector2D(-8, 0), 0.8f);
                GetGameContext()->SpawnEntity(beam);
            }

            particlesFire = GetGameContext()->GetParticleRoot()->CreateParticles("laser_cannon_fire", Vector2D(8, 0));
            if (!particlesFire.isNull())
            {
                particlesFire->SetParticleParent(GetShip());
            }

            AudioManager::GetInstance()->PlaySoundSample("laser/laser_cannon_shoot.wav", 0.45f);
            playerLastDirectionSign = playerDirectionSign;
            return;
        }

        if (!IsShipFullyVisible())
        {
            playerLastDirectionSign = playerDirectionSign;
            return;
        }

        if (startTime > gameTime
                && (playerDirectionSign == playerLastDirectionSign || preventDirectionTriggerTime > gameTime))
        {
            return;
        }

        playerLastDirectionSign = playerDirectionSign;

        startTime = gameTime + 1.6f + qfrand() * 3.0f;
        preventDirectionTriggerTime = gameTime + 2.0f;
        waitTime = gameTime + 1.45f;
        chargeTime = gameTime + 0.7f;

        AudioManager::GetInstance()->PlaySoundSample("laser/laser_cannon_charge.wav", 0.45f);

        particlesCharge = GetGameContext()->GetParticleRoot()->CreateParticles("laser_charge_green", Vector2D(8, 0));
        if (!particlesCharge.isNull())
        {
            particlesCharge->SetParticleParent(GetShip());
        }
    }

    virtual void OnMove(float frametime)
    {

    }
};

REGISTER_AI(Fighter3AI, fighter3ai);

