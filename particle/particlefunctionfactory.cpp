#include "particlebase.h"

#include "factory.h"

ParticleFunctionFactory ParticleFunctionFactory::instance;

ParticleFunctionFactory *ParticleFunctionFactory::GetInstance()
{
    return &instance;
}

static struct function_register_particle_function
    : public function_register_template< ParticleFunctionFactory::FunctionFactory, function_register_particle_function >
{
} firstFactory;

ParticleFunctionFactory::ParticleFunctionFactory()
{
}

ParticleFunctionFactory::~ParticleFunctionFactory()
{
    Shutdown();
}

void ParticleFunctionFactory::Shutdown()
{
    DestroyFactoryList(&firstFactory);
}

void ParticleFunctionFactory::RegisterFactoryFunction(FunctionFactory factory, const char *name)
{
    RegisterFactory(&firstFactory, factory, name);
}

IParticleFunction *ParticleFunctionFactory::CreateParticleFunction(const char *name)
{
    return ExecuteFactoryFunction< function_register_particle_function, IParticleFunction >(&firstFactory, name);
}
