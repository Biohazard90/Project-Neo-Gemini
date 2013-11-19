#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "destructible.h"
#include "gameutil.h"

class Obstacle : public Destructible
{
    DECLARE_CLASS(Obstacle, Destructible);

public:
    Obstacle();

    virtual bool IsObstacle() const { return true; }

    virtual void OnSimulate(float frametime);

    virtual bool ShouldCollide(ICollidable *) const;
    virtual bool IsSolid() const { return true; }
    virtual void OnCollision(ICollidable *);

    virtual void Init(const Resource_Obstacle_t &data);

private:

    Resource_Obstacle_t data;
};

#endif // OBSTACLE_H
