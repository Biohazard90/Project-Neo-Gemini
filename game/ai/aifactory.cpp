#include "aifactory.h"

#include "macros.h"
#include "factory.h"

#include <QDebug>

AIFactory AIFactory::instance;
AIFactory *AIFactory::GetInstance()
{
    return &instance;
}

static struct function_register_ai_t
    : public function_register_template< AIFactory::AIFactoryFunction, function_register_ai_t >
{
} firstFactory_ai;

AIFactory::AIFactory()
{
}

AIFactory::~AIFactory()
{
}

void AIFactory::Init()
{
    ConvertFactoryListToHash(&firstFactory_ai, functions_ai);
}

void AIFactory::RegisterAIFactory(AIFactoryFunction factory, const char *scriptName)
{
    RegisterFactory(&firstFactory_ai, factory, scriptName);
}

IAI *AIFactory::CreateAIByName(const QString &scriptName)
{
    if (!functions_ai.contains(scriptName))
    {
        DBGWARNING("!! Unknown ai factory function:" << scriptName);
        return nullptr;
    }

    AIFactoryFunction func = functions_ai[scriptName];
    return func();
}
