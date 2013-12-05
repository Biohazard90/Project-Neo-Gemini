#include "entityfactory.h"
#include "factory.h"

EntityFactory EntityFactory::instance;
EntityFactory *EntityFactory::GetInstance()
{
    return &instance;
}

static struct function_register_entity
    : public function_register_template< EntityFactory::EntityFactoryFunction, function_register_entity >
{
} firstFactory;


EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{
}

void EntityFactory::Init()
{
    ConvertFactoryListToHash(&firstFactory, functions);
}

void EntityFactory::RegisterEntityFactory(EntityFactoryFunction factory, const char *scriptName)
{
    RegisterFactory(&firstFactory, factory, scriptName);
}

Entity *EntityFactory::CreateEntityByName(const QString &scriptName)
{
    if (!functions.contains(scriptName))
        return nullptr;

    EntityFactoryFunction func = functions[scriptName];
    return func();
}
