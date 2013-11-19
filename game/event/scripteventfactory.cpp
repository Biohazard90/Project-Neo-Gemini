#include "scripteventfactory.h"

#include "macros.h"
#include "factory.h"

#include <QDebug>

ScriptEventFactory ScriptEventFactory::instance;
ScriptEventFactory *ScriptEventFactory::GetInstance()
{
    return &instance;
}

static struct function_register_script_event
    : public function_register_template< ScriptEventFactory::ScriptEventFactoryFunction, function_register_script_event >
{
} firstFactory;


ScriptEventFactory::ScriptEventFactory()
{
}

ScriptEventFactory::~ScriptEventFactory()
{
}

void ScriptEventFactory::Init()
{
    ConvertFactoryListToHash(&firstFactory, functions);
}

void ScriptEventFactory::RegisterScriptEventFactory(ScriptEventFactoryFunction factory, const char *scriptName)
{
    RegisterFactory(&firstFactory, factory, scriptName);
}

IScriptEvent *ScriptEventFactory::CreateScriptEventByName(const QString &scriptName)
{
    if (!functions.contains(scriptName))
    {
        DBGWARNING("!! Unknown script event function:" << scriptName);
        return NULL;
    }

    ScriptEventFactoryFunction func = functions[scriptName];
    return func();
}
