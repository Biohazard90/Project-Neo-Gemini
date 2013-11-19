
#include "base.h"
#include "gamebase.h"
#include "aibase.h"


#define SHOOT_POS_INNER_CENTER Vector2D(20.0f, 0.0f)
#define SHOOT_POS_INNER_LEFT_0 Vector2D(12.5f, -30.0f)
#define SHOOT_POS_INNER_LEFT_1 Vector2D(-16.5f, -46.0f)
#define SHOOT_POS_INNER_RIGHT_0 Vector2D(12.5f, 30.0f)
#define SHOOT_POS_INNER_RIGHT_1 Vector2D(-16.5f, 46.0f)

#define SHOOT_POS_OUTER_LEFT_0 Vector2D(44.0f, -35.0f)
#define SHOOT_POS_OUTER_LEFT_1 Vector2D(34.0f, -49.0f)
#define SHOOT_POS_OUTER_RIGHT_0 Vector2D(44.0f, 35.0f)
#define SHOOT_POS_OUTER_RIGHT_1 Vector2D(34.0f, 49.0f)

#define DEFAULT_X_DIST 110.0f

class Boss0AI : public AbstractAI
{
    float frametime;

    float stateTimer;
    int state;

    Vector2D targetPosition;
    bool dashing;
    float timer0;
    bool directionToggle;

    bool shootSlotToggle;
    int salvoCount;
    int routeCount;

    QPointer<ParticleSystem> particlesIdle;

    Resource_Projectile_t projectileFast;
    Resource_Projectile_t projectileSlow;
    Resource_Projectile_t projectileSpam;

    Resource_Fighter_t fighterSwarm;
    Resource_Fighter_t fighterSpam;

public:
    virtual void Spawn()
    {
        particlesIdle = GetGameContext()->GetParticleRoot()->CreateParticles("boss_0_idle");
        if (!particlesIdle.isNull())
        {
            particlesIdle->SetParticleParent(GetShip());
        }

        projectileFast = Resource::GetInstance()->GetProjectile("boss0fast");
        projectileSlow = Resource::GetInstance()->GetProjectile("boss0slow");
        projectileSpam = Resource::GetInstance()->GetProjectile("boss0spam");
        fighterSwarm = Resource::GetInstance()->GetFighter("fighter_3");
        fighterSpam = Resource::GetInstance()->GetFighter("fighter_0");

        GetShip()->SetVelocity(Vector2D(0, 0));

        stateTimer = GetGameContext()->GetGameTime() + 2.0f;
        state = 0;

        Reset();

        directionToggle = qbrand();
    }

    virtual void OnRemove()
    {
        if (!particlesIdle.isNull())
            particlesIdle->StopEmissionAndParticles();
    }

    virtual void OnSimulate(float frametime)
    {
        this->frametime = frametime;

        if (!GetShip()->IsAlive())
        {
            float angle = GetShip()->GetAngle();
            float sign = directionToggle ? 1 : -1;
            GetShip()->SetAngle(angle + frametime * 4.5f * sign);
            Accelerate(Vector2D(-25, 0), 5.0f);
            return;
        }

        //TODO: scriptable boss states
        //TODO: use state pattern
        switch (state)
        {
        case 0:
            StateIntro();
            break;
        case 1:
            StateDash();
            break;
        case 2:
            StateSwarm();
            break;
        case 3:
            StateDash();
            break;
        case 4:
            StateDiamondRoute();
            break;
        }

        UpdateSway();
    }

    virtual void OnMove(float frametime)
    {
    }

    void UpdateState()
    {
        const float time = GetGameContext()->GetGameTime();

        if (stateTimer < time)
        {
            NextState();
        }
    }

    void NextState()
    {
        const float time = GetGameContext()->GetGameTime();

        state++;

        if (state >= 5)
            state = 1;

        stateTimer = time + 15.0f + qfrand() * 10.0f;

        GetShip()->SetShootDelay(0);

        Reset();
    }

    void Reset()
    {
        dashing = false;
        targetPosition.Init();
        salvoCount = 0;
        directionToggle = false;
        timer0 = GetGameContext()->GetGameTime();
        shootSlotToggle = qbrand();
        routeCount = 0;
    }

    void StateIntro()
    {
        MoveToPosition(Vector2D(DEFAULT_X_DIST, 0), true, 150.0f);
        UpdateState();
    }

    void StateDash()
    {
        Player *player = GetGameContext()->GetPlayer();
        const float halfSizeX = GetShip()->GetSize().x * 0.5f;
        const float halfSizeY = GetShip()->GetSize().y * 0.5f;
        const float time = GetGameContext()->GetGameTime();

        if (!dashing
                && timer0 < time
                && player != NULL && player->IsAlive())
        {
            float playerDistanceY = abs(player->GetOrigin().y - GetShip()->GetOrigin().y);

            if (playerDistanceY < halfSizeY)
            {
                dashing = true;
                targetPosition = Vector2D(halfSizeX, GetShip()->GetOrigin().y);
                GetShip()->SetShootDelay(0.12f);
            }
        }

        if (dashing)
        {
            targetPosition.y = GetShip()->GetOrigin().y;
            MoveToPosition(targetPosition, false, 500.0f);

            FireOuterFast();

            if (IsAtPosition(targetPosition, false, 80.0f))
            {
                dashing = false;
                timer0 = time + 6.0f;
            }
        }
        else
        {
            float xDistance = Camera::GetInstance()->GetWorldMaxs().x - DEFAULT_X_DIST;
            if (GetShip()->GetOrigin().x < xDistance - 5.0f)
            {
                MoveToPosition(Vector2D(DEFAULT_X_DIST, targetPosition.y), true, 250.0f);
            }
            else
            {
                MoveUpAndDown(300.0f);

                FireInnerAim();

                UpdateState();
            }
        }
    }

    void StateSwarm()
    {
        const float time = GetGameContext()->GetGameTime();
        const float initialDelay = 2.0f;
        const float swarmDelay = 1.25f;

        MoveToPosition(Vector2D(DEFAULT_X_DIST, 0), true);

        if ((time - timer0) < initialDelay)
            return;

        if ((time - (timer0 + initialDelay + salvoCount * swarmDelay)) < 0.0f)
            return;

        const float worldMinY = Camera::GetInstance()->GetWorldMins().y;
        const float worldMaxY = Camera::GetInstance()->GetWorldMaxs().y;

        EnemyShip *fighter = CreateSwarmFighter(fighterSwarm);

        if (fighter != NULL)
        {
            float fraction = 0.07f * (salvoCount/2);

            if (salvoCount % 2 == 0)
                fraction = 0.35f - fraction;
            else
                fraction = 0.65f + fraction;

            Vector2D origin = fighter->GetOrigin();
            origin.y = qlerp(fraction, worldMinY, worldMaxY);
            fighter->Teleport(origin);

            GetGameContext()->SpawnEntity(fighter);
        }

        salvoCount++;

        if (salvoCount >= 8)
        {
            NextState();
        }
    }

    void StateDiamondRoute()
    {
        if (routeCount == 0)
        {
            const Vector2D initialPos = Vector2D(DEFAULT_X_DIST, 0);

            MoveToPosition(initialPos, true, 100.0f);

            if (!IsAtPosition(initialPos, true, 20.0f))
                return;

            routeCount++;
        }

        const float time = GetGameContext()->GetGameTime();
        const Vector2D mins = Camera::GetInstance()->GetWorldMins();
        const Vector2D maxs = Camera::GetInstance()->GetWorldMaxs();
        const Vector2D worldHalf = mins + (maxs - mins) * 0.5f;
        const Vector2D sizeHalf = GetShip()->GetSize() * 0.5f;

        const Vector2D checkpoints[] =  {
            Vector2D(worldHalf.x, maxs.y - sizeHalf.y),
            Vector2D(mins.x + sizeHalf.x, worldHalf.y),
            Vector2D(worldHalf.x, mins.y + sizeHalf.y),
            Vector2D(maxs.x - sizeHalf.x, worldHalf.y),
        };

        const Vector2D &target = checkpoints[routeCount - 1];

        MoveToPosition(target, false, 120.0f);

        FireInnerSpam();

        if (timer0 < time - 3.0f)
        {
            EnemyShip *fighter = CreateSwarmFighter(fighterSpam);
            if (fighter != NULL)
            {
                Vector2D origin = fighter->GetOrigin();
                origin.y = qlerp(qfrand(), mins.y + fighter->GetSize().y * 0.5f,
                                 maxs.y - fighter->GetSize().y * 0.5f);
                fighter->SetOrigin(origin);
                GetGameContext()->SpawnEntity(fighter);
            }

            timer0 = time;
        }

        if ((target - GetShip()->GetOrigin()).LengthSqr() < 20.0f)
        {
            routeCount++;

            if (routeCount >= QARRAYSIZE(checkpoints) + 1)
            {
                NextState();
            }
        }
    }

    EnemyShip *CreateSwarmFighter(const Resource_Fighter_t &data)
    {
        EnemyShip *fighter = (EnemyShip*)GetGameContext()->CreateEntityNoSpawn("fighter");

        if (fighter == NULL)
            return NULL;

        IAI *ai = AIFactory::GetInstance()->CreateAIByName(data.ai.c_str());

        fighter->Init(data, ai);

        Vector2D origin = fighter->GetOrigin();
        origin.y = 0.0f;
        fighter->Teleport(origin);

        fighter->SetAngle(-180);

        return fighter;
    }

    void FireOuterFast()
    {
        if (!GetShip()->CanShoot())
        {
            return;
        }

        if (shootSlotToggle)
        {
            GetShip()->Shoot(Vector2D(-1, 0), projectileFast, SHOOT_POS_OUTER_LEFT_0);
            GetShip()->Shoot(Vector2D(-1, 0), projectileFast, SHOOT_POS_OUTER_RIGHT_0);
        }
        else
        {
            GetShip()->Shoot(Vector2D(-1, 0), projectileFast, SHOOT_POS_OUTER_LEFT_1);
            GetShip()->Shoot(Vector2D(-1, 0), projectileFast, SHOOT_POS_OUTER_RIGHT_1);
        }

        shootSlotToggle = !shootSlotToggle;
    }

    void FireInnerAim()
    {
        if (!GetShip()->CanShoot())
        {
            return;
        }

        if (shootSlotToggle)
        {
            ShootAimed(projectileSlow, SHOOT_POS_INNER_LEFT_0);
            ShootAimed(projectileSlow, SHOOT_POS_INNER_RIGHT_0);
        }
        else
        {
            ShootAimed(projectileSlow, SHOOT_POS_INNER_LEFT_1);
            ShootAimed(projectileSlow, SHOOT_POS_INNER_RIGHT_1);
        }

        shootSlotToggle = !shootSlotToggle;
    }

    void FireInnerSpam()
    {
        if (!GetShip()->CanShoot())
            return;

        const int maxOffsetCount = 5;
        const float offsetDegrees = 360.0f / maxOffsetCount;
        const float shootOffset = offsetDegrees * salvoCount;

        if (shootSlotToggle)
        {
            const float slotOffsetDegrees = 360.0f / 3;

            GetShip()->Shoot(Vector2D::AngleDirection(shootOffset), projectileSpam, SHOOT_POS_INNER_CENTER);
            GetShip()->Shoot(Vector2D::AngleDirection(shootOffset + slotOffsetDegrees), projectileSpam, SHOOT_POS_INNER_LEFT_1);
            GetShip()->Shoot(Vector2D::AngleDirection(shootOffset + slotOffsetDegrees * 2), projectileSpam, SHOOT_POS_INNER_RIGHT_1);
        }
        else
        {
            const float slotOffsetDegrees = 360.0f / 2;

            GetShip()->Shoot(Vector2D::AngleDirection(shootOffset), projectileSpam, SHOOT_POS_INNER_LEFT_0);
            GetShip()->Shoot(Vector2D::AngleDirection(shootOffset + slotOffsetDegrees), projectileSpam, SHOOT_POS_INNER_RIGHT_0);
        }

        shootSlotToggle = !shootSlotToggle;
        salvoCount++;
        if (salvoCount >= maxOffsetCount)
            salvoCount = 0;
    }

    bool IsAtPosition(Vector2D position, bool rightAligned = false, float tolerance = 10.0f)
    {
        if (rightAligned)
        {
            Vector2D worldMaxs = Camera::GetInstance()->GetWorldMaxs();
            position.x = worldMaxs.x - position.x;
        }

        return (position - GetShip()->GetOrigin()).LengthSqr() < tolerance;
    }

    void ShootAimed(const Resource_Projectile_t &projectile, Vector2D origin)
    {
        Player *player = GetGameContext()->GetPlayer();

        Vector2D targetOrigin = player ? player->GetOrigin() : (origin - Vector2D(100, 0));
        Vector2D targetVelocity = player ? player->GetVelocity() : vec2_origin;
        Vector2D predicted;

        Vector2D originWorldspace = origin;
        originWorldspace.Rotate(GetShip()->GetAngle());
        originWorldspace += GetShip()->GetOrigin();

        if (!player || !player->IsAlive()
                || !PredictProjectileTarget(originWorldspace, targetOrigin, targetVelocity, projectile.speed, predicted))
            predicted = originWorldspace - Vector2D(100, 0);

        Vector2D direction = predicted - originWorldspace;
        direction.Normalize();

        GetShip()->Shoot(direction, projectile, origin);
    }

    virtual void MoveUpAndDown(float speed = 300.0f)
    {
        const float halfSizeY = GetShip()->GetSize().y * 0.5f;
        const float originY = GetShip()->GetOrigin().y;

        Vector2D target(DEFAULT_X_DIST, 0);

        if (directionToggle)
            target.y = Camera::GetInstance()->GetWorldMins().y;
        else
            target.y = Camera::GetInstance()->GetWorldMaxs().y;

        MoveToPosition(target, true, speed);

        if (directionToggle && originY - halfSizeY < Camera::GetInstance()->GetWorldMins().y)
            directionToggle = false;
        else if (!directionToggle && originY + halfSizeY > Camera::GetInstance()->GetWorldMaxs().y)
            directionToggle = true;
    }

    virtual void MoveToPosition(Vector2D position, bool rightAligned = false,
                                float speed = 300.0f, float acceleration = 7.0f)
    {
        if (rightAligned)
        {
            Vector2D worldMaxs = Camera::GetInstance()->GetWorldMaxs();
            position.x = worldMaxs.x - position.x;
        }

        Vector2D velocity = position - GetShip()->GetOrigin();

        if (velocity.LengthSqr() < 1.0f)
        {
            GetShip()->SetOrigin(position);
        }
        else
        {
            float maxDist = velocity.Normalize() * 2.0f;
            velocity *= MIN(maxDist, speed);
            Accelerate(velocity, acceleration);
        }
    }

    virtual void Accelerate(Vector2D desiredVelocity, float acceleration)
    {
        Vector2D lastVelocity(GetShip()->GetVelocity());
        Vector2D delta = desiredVelocity - lastVelocity;

        float length = delta.Normalize();
        float speed = length * frametime * acceleration;
        speed = MIN(speed, length);

        lastVelocity += delta * speed;

        GetShip()->SetVelocity(lastVelocity);
    }

    virtual void UpdateSway()
    {
        float currentAngle = GetShip()->GetAngle();
        float desiredAngle = QREMAPVAL(GetShip()->GetVelocity().y,
                                              -200, 200,
                                              3, -3);
        desiredAngle -= 180.0f;
        currentAngle = qapproachlinear(desiredAngle, currentAngle, frametime * 200);
        GetShip()->SetAngle(currentAngle);
    }

    bool SolveQuadratic( float a, float b, float c, Vector2D &out )
    {
        bool result = false;

        if ( abs(a) < 1e-6 )
        {
            if ( abs(b) < 1e-6 )
            {
                if ( abs(c) < 1e-6 )
                {
                    result = true;
                    out.Init();
                }
            }
            else
            {
                result = true;
                out.Init( -c / b, -c / b );
            }
        }
        else
        {
            float disc = ( b * b ) - ( 4.0f * a * c );

            if ( disc >= 0 )
            {
                disc = sqrtf( disc );
                a = 2.0f * a;
                out.Init( ( -b -disc ) / a, ( -b + disc ) / a );
                result = true;
            }
        }

        return result;
    }

    bool PredictProjectileTarget( Vector2D projectileOrigin, Vector2D targetOrigin, Vector2D targetLinearVelocity,
                                    float linearProjectileSpeed, Vector2D &predictedTarget )
    {
        bool result = false;
        Vector2D delta = targetOrigin - projectileOrigin;

        float a = targetLinearVelocity.LengthSqr() - linearProjectileSpeed * linearProjectileSpeed;
        float b = 2 * ( targetLinearVelocity.x * delta.x
            + targetLinearVelocity.y * delta.y );
        float c = delta.LengthSqr();

        Vector2D ts;
        bool solved = SolveQuadratic( a, b, c, ts );

        if ( solved )
        {
            float t0 = ts[0];
            float t1 = ts[1];
            float t = MIN( t0, t1 );

            if ( t < 0 )
            {
                t = MAX( t0, t1 );
            }

            if ( t > 0 )
            {
                result = true;
                predictedTarget = targetOrigin + targetLinearVelocity * t;
            }
        }

        return result;
    }
};

REGISTER_AI(Boss0AI, boss0ai);
