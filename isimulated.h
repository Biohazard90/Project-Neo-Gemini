#ifndef ISIMULATED_H
#define ISIMULATED_H

class ISimulated
{
public:
    virtual ~ISimulated(){}

    virtual void OnSimulate(float frametime) = 0;
};

#endif // ISIMULATED_H
