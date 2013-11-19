#ifndef SCRIPTEVENTFACTORY_H
#define SCRIPTEVENTFACTORY_H

#include <QHash>

#include "iscriptevent.h"


#define REGISTER_SCRIPT_EVENT(className, scriptName) \
    static IScriptEvent *__pfuncCreateScriptEvent##className() \
    { \
        return new className(); \
    } \
    static class __pregisterScriptEvent##className \
    { \
        public: \
        __pregisterScriptEvent##className() \
        { \
            ScriptEventFactory::RegisterScriptEventFactory( \
                __pfuncCreateScriptEvent##className,#scriptName); \
        } \
    }__g_instanceScriptEvent##className

class ScriptEventFactory
{
    static ScriptEventFactory instance;
    ScriptEventFactory();
    ~ScriptEventFactory();

public:
    static ScriptEventFactory *GetInstance();

    void Init();

    typedef IScriptEvent*(*ScriptEventFactoryFunction)();
    static void RegisterScriptEventFactory(ScriptEventFactoryFunction factory, const char *scriptName);

    IScriptEvent *CreateScriptEventByName(const QString &scriptName);

private:

    QHash<QString, ScriptEventFactoryFunction> functions;
};


#endif // SCRIPTEVENTFACTORY_H
