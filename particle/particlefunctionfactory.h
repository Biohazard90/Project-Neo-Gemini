#ifndef PARTICLEFUNCTIONFACTORY_H
#define PARTICLEFUNCTIONFACTORY_H

#include "iparticlefunction.h"


#define REGISTER_PARTICLE_FUNCTION(className, scriptName) \
    static IParticleFunction *__pfuncCreate##className() \
    { \
        return new className(); \
    } \
    static class __pregister##className \
    { \
        public: \
        __pregister##className() \
        { \
            ParticleFunctionFactory::RegisterFactoryFunction( \
                __pfuncCreate##className,#scriptName); \
        } \
    }__g_instance##className


class ParticleFunctionFactory
{
    static ParticleFunctionFactory instance;
    ParticleFunctionFactory();
    ~ParticleFunctionFactory();
public:
    static ParticleFunctionFactory *GetInstance();

    void Shutdown();

    typedef IParticleFunction*(*FunctionFactory)();
    static void RegisterFactoryFunction(FunctionFactory factory, const char *name);

    IParticleFunction *CreateParticleFunction(const char *name);
};

#endif // PARTICLEFUNCTIONFACTORY_H
