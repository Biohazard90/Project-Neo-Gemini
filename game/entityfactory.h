#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <QHash>

#include "entity.h"

#define REGISTER_ENTITY_CLASS(className, scriptName) \
    const char *className::constClassName = #scriptName; \
    static Entity *__pfuncCreateEntity##className() \
    { \
        return new className(); \
    } \
    static class __pregister##className \
    { \
        public: \
        __pregister##className() \
        { \
            EntityFactory::RegisterEntityFactory( \
                __pfuncCreateEntity##className,#scriptName); \
        } \
    }__g_instance##className

class EntityFactory
{
    static EntityFactory instance;
    EntityFactory();
    ~EntityFactory();

public:
    static EntityFactory *GetInstance();

    void Init();

    typedef Entity*(*EntityFactoryFunction)();
    static void RegisterEntityFactory(EntityFactoryFunction factory, const char *scriptName);

    Entity *CreateEntityByName(const QString &scriptName);

private:

    QHash<QString, EntityFactoryFunction> functions;
};



#endif // ENTITYFACTORY_H
