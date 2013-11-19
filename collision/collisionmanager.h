#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <QVector>

#include "vector2d.h"
#include "icollidable.h"

struct render_context_t;

#define MAX_COLLISION_STEPS 10

class CollisionManager
{
public:
    CollisionManager();
    ~CollisionManager();

    void AddObject(ICollidable *object);
    void RemoveObject(ICollidable *object);
    void TeleportObject(ICollidable *object);

    void OnSimulate(float frametime);
    void PaintCollisions(const render_context_t &context);

private:
    struct CollidableData_t
    {
        Vector2D lastPos;
        Vector2D tracePos;
        ICollidable *object;

        int stepCount;
        Vector2D steps[MAX_COLLISION_STEPS][2];
    };

    QVector<CollidableData_t*> objects;
};

#endif // COLLISIONMANAGER_H
