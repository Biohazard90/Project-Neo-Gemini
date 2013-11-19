#ifndef FACTORY_H
#define FACTORY_H

#include <QHash>
#include <QString>

#include "macros.h"

template< class T, class C >
struct function_register_template
{
    T function;
    const char *nameLiteral;
    C *next;
};

template< class T, class D >
FORCEINLINE void ConvertFactoryListToHash(D *first,
                                           QHash<QString, T> &hash)
{
    D *cur = first->next;
    while (cur != NULL)
    {
        D *deleteMe = cur;

        hash.insert(cur->nameLiteral, cur->function);

        cur = cur->next;
        delete deleteMe;
    }
    first->next = NULL;
}

template< class T, class D >
FORCEINLINE void RegisterFactory(D *first,
                                 T function, const char *name)
{
    D *f = new D;
    f->next = NULL;
    f->nameLiteral = name;
    f->function = function;

    D *cur = first;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = f;
}

template< class D >
FORCEINLINE void DestroyFactoryList(D *first)
{
    D *cur = first->next;
    first->next = NULL;

    while ( cur != NULL )
    {
        D *deleteMe = cur;
        cur = cur->next;
        delete deleteMe;
    }
}

template< class D, class R >
FORCEINLINE R *ExecuteFactoryFunction(D *first, const char *name)
{
    D *cur = first->next;

    while ( cur != NULL )
    {
        if ( qstricmp( cur->nameLiteral, name ) == 0 )
            return cur->function();

        cur = cur->next;
    }

    return NULL;
}

#endif // FACTORY_H
