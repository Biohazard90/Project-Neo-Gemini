#include "collisionmanager.h"

#include "base.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
    DESTROY_QVECTOR(objects);
}

void CollisionManager::AddObject(ICollidable *object)
{
#if DEBUG
    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        Q_ASSERT(o->object != object);
    }
    FOREACH_QVECTOR_FAST_END;
#endif

    CollidableData_t *data = new CollidableData_t;
    data->object = object;
    data->lastPos = object->GetCollisionOrigin();
    data->tracePos = data->lastPos;
    data->stepCount = 0;
    objects.push_back(data);
}

void CollisionManager::RemoveObject(ICollidable *object)
{
    int i = -1;

    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        if (o->object == object)
        {
            i = io;
            break;
        }
    }
    FOREACH_QVECTOR_FAST_END;

    if (i >= 0)
    {
        delete objects[i];
        objects.remove(i);
    }
}

void CollisionManager::TeleportObject(ICollidable *object)
{
    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        if (o->object == object)
        {
            o->lastPos = object->GetCollisionOrigin();
            o->tracePos = o->lastPos;
            o->stepCount = 0;
            break;
        }
    }
    FOREACH_QVECTOR_FAST_END;
}

void CollisionManager::OnSimulate(float frametime)
{
    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        o->tracePos = o->lastPos;

        if (!o->object->IsSolid())
        {
            o->stepCount = 0;
            continue;
        }

        Vector2D mins, maxs;
        o->object->GetCollisionBounds(mins, maxs);

        float stepSizeA = MIN(abs(mins.x), abs(mins.y));
        float stepSizeB = MIN(maxs.x, maxs.y);

        stepSizeA = MIN(stepSizeA, stepSizeB) * 1.95f;

        Vector2D origin = o->object->GetCollisionOrigin();
        Vector2D delta = origin - o->lastPos;
        float requiredLength = delta.Normalize();

        o->stepCount = 0;
        for (int i = 0; i < MAX_COLLISION_STEPS - 1; i++)
        {
            float stepSize = (i + 1) * stepSizeA;
            if (stepSize >= requiredLength)
                break;

            o->stepCount++;

            Vector2D localDelta = delta * stepSize;
            o->steps[i][0] = mins + o->lastPos + localDelta;
            o->steps[i][1] = maxs + o->lastPos + localDelta;
        }

        o->steps[o->stepCount][0] = mins + origin;
        o->steps[o->stepCount][1] = maxs + origin;
        o->stepCount++;
    }
    FOREACH_QVECTOR_FAST_END;

    QHash<ICollidable*, QHash<ICollidable*, bool> > collisions;

    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o1)
    {
        if (!o1->object->IsSolid())
            continue;

        FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o2)
        {
            if (!o2->object->IsSolid())
                continue;

            if (collisions.contains(o1->object)
                    && collisions.value(o1->object).contains(o2->object))
                continue;

            if (!o1->object->ShouldCollide(o2->object)
                    || !o2->object->ShouldCollide(o1->object))
                continue;

            for (int i1 = 0; i1 < o1->stepCount; i1++)
            {
                for (int i2 = 0; i2 < o2->stepCount; i2++)
                {
                    if (IntersectBounds(o1->steps[i1][0], o1->steps[i1][1],
                                        o2->steps[i2][0], o2->steps[i2][1]))
                    {
                        collisions[o2->object].insert(o1->object, true);

                        o1->object->OnCollision(o2->object);
                        o2->object->OnCollision(o1->object);

                        i1 = MAX_COLLISION_STEPS;
                        i2 = MAX_COLLISION_STEPS;
                    }
                }
            }
        }
        FOREACH_QVECTOR_FAST_END;
    }
    FOREACH_QVECTOR_FAST_END;

    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        o->lastPos = o->object->GetCollisionOrigin();
    }
    FOREACH_QVECTOR_FAST_END;
}

void CollisionManager::PaintCollisions(const render_context_t &context)
{
    context.painter->save();

    context.painter->setBrush(Qt::transparent);

    FOREACH_QVECTOR_FAST(objects, CollidableData_t*, o)
    {
        if (!o->object->IsSolid())
            continue;

        Vector2D mins, maxs;
        o->object->GetCollisionBounds(mins, maxs);

        mins += o->tracePos;
        maxs += o->tracePos;

        Camera::GetInstance()->ToScreen(mins);
        Camera::GetInstance()->ToScreen(maxs);

        maxs -= mins;
        context.painter->setPen(Qt::red);
        context.painter->drawRect(mins.x, mins.y,
                                  maxs.x - 1, maxs.y - 1);


        o->object->GetCollisionBounds(mins, maxs);

        mins += o->object->GetCollisionOrigin();
        maxs += o->object->GetCollisionOrigin();

        Camera::GetInstance()->ToScreen(mins);
        Camera::GetInstance()->ToScreen(maxs);

        maxs -= mins;
        context.painter->setPen(Qt::green);
        context.painter->drawRect(mins.x, mins.y,
                                  maxs.x - 1, maxs.y - 1);

        context.painter->setPen(Qt::yellow);
        for (int i = 0; i < o->stepCount - 1; i++)
        {
            mins = o->steps[i][0];
            maxs = o->steps[i][1];

            Camera::GetInstance()->ToScreen(mins);
            Camera::GetInstance()->ToScreen(maxs);

            maxs -= mins;
            context.painter->drawRect(mins.x, mins.y,
                                      maxs.x - 1, maxs.y - 1);
        }
    }
    FOREACH_QVECTOR_FAST_END;

    context.painter->restore();
}
