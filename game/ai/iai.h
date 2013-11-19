#ifndef IAI_H
#define IAI_H

class Ship;
class IGameContext;

class IAI
{
public:
    virtual ~IAI(){}

    virtual void Init(Ship *ship, IGameContext *gameContext) = 0;

    virtual void Spawn() = 0;
    virtual void OnRemove() = 0;
    virtual void OnSimulate(float frametime) = 0;
    virtual void OnMove(float frametime) = 0;
};

#endif // IAI_H
