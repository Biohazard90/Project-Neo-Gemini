#ifndef AIFACTORY_H
#define AIFACTORY_H

#include <QHash>

#include "iai.h"

#define REGISTER_AI(className, scriptName) \
    static IAI *__pfuncCreateAI##className() \
    { \
        return new className(); \
    } \
    static class __pregisterAI##className \
    { \
        public: \
        __pregisterAI##className() \
        { \
            AIFactory::RegisterAIFactory( \
                __pfuncCreateAI##className,#scriptName); \
        } \
    }__g_instanceAI##className

class AIFactory
{
    static AIFactory instance;
    AIFactory();
    ~AIFactory();

public:
    static AIFactory *GetInstance();

    void Init();

    typedef IAI*(*AIFactoryFunction)();

    static void RegisterAIFactory(AIFactoryFunction factory, const char *scriptName);

    IAI *CreateAIByName(const QString &scriptName);

private:

    QHash<QString, AIFactoryFunction> functions_ai;
};

#endif // AIFACTORY_H
