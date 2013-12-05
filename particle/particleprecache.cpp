#include "particlebase.h"
#include <QtXml>

ParticlePrecache ParticlePrecache::instance;

ParticlePrecache *ParticlePrecache::GetInstance()
{
    return &instance;
}

ParticlePrecache::ParticlePrecache()
{
}

ParticlePrecache::~ParticlePrecache()
{
    QHashIterator<QString,ParticleSystem*> itr(systems);
    while (itr.hasNext())
        delete itr.next().value();
}

void ParticlePrecache::PrecacheParticleSystem(const char *path)
{
    ParticleSystem *system = ParseParticleSystem(path);

    if (system != nullptr)
    {
        QFileInfo inf(path);
        QString name = inf.baseName();

        if (systems.contains(name))
        {
            DBGWARNING("!! Duplicate particle system:" << name);
        }
        else
        {
            QFileInfo inf( path );
            systems.insert(name, system);
        }
    }
}

ParticleSystem *ParticlePrecache::CreateParticleSystem(const char *name)
{
    ParticleSystem *particle = GET_QHASH(systems, name);

    if (particle)
    {
        return CreateSystemCopy(particle);
    }
    else
    {
        DBGWARNING("!! Particle not precached:" << name);
        return nullptr;
    }
}

ParticleSystem *ParticlePrecache::CreateSystemCopy(ParticleSystem *src)
{
    ParticleSystem *p = new ParticleSystem();

    p->origin = src->origin;
    p->forward = src->forward;

    CreateFunctionCopy<IParticleEmitter*>(src->emitters, p->emitters);
    CreateFunctionCopy<IParticleInitializer*>(src->initializers, p->initializers);
    CreateFunctionCopy<IParticleOperator*>(src->operators, p->operators);
    CreateFunctionCopy<IParticleRenderer*>(src->renderers, p->renderers);

    FOREACH_QVECTOR_FAST(src->children, ParticleSystem*, s)
    {
        ParticleSystem *child = CreateSystemCopy(s);
        p->children.push_back(child);
    }
    FOREACH_QVECTOR_FAST_END;

    return p;
}

template< class T >
void ParticlePrecache::CreateFunctionCopy(QVector<T> &source, QVector<T> &dest)
{
    FOREACH_QVECTOR(source, T, n)
    {
        T f = (T)n->Copy();
        dest.append(f);
    }
    FOREACH_QVECTOR_END;
}

ParticleSystem *ParticlePrecache::ParseParticleSystem(const char *path)
{
    QDomElement root;
    QString fullPath(PATH_PARTICLE_ROOT);
    fullPath += path;

    if (!OpenXMLFile(fullPath, root))
    {
        DBGWARNING("!! Error loading particle file:" << path);
        return nullptr;
    }

    ParticleSystem *system = new ParticleSystem();

    QDomElement childFunctions = root.firstChildElement("emitters");
    ParseSiblingFunctions< IParticleEmitter* >(&childFunctions, system->emitters);

    childFunctions = root.firstChildElement("initializers");
    ParseSiblingFunctions< IParticleInitializer* >(&childFunctions, system->initializers);

    childFunctions = root.firstChildElement("operators");
    ParseSiblingFunctions< IParticleOperator* >(&childFunctions, system->operators);

    childFunctions = root.firstChildElement("renderers");
    ParseSiblingFunctions< IParticleRenderer* >(&childFunctions, system->renderers);

    QDomNodeList childrenNodes = root.firstChildElement("children").childNodes();
    FOREACH_QDOM_NODE(childrenNodes, childNode)
    {
        Vector2D childOrigin(vec2_origin);

        std::string strChild = "";

        if (childNode.firstChild().isText())
        {
            strChild = childNode.text().toStdString();
        }
        else
        {
            strChild = childNode.firstChildElement("name").text().toStdString();
            childOrigin = XMLParseVector(childNode.firstChildElement("origin"));
        }

        ParticleSystem *childSystem = ParseParticleSystem(strChild.c_str());

        if (childSystem != nullptr)
        {
            childSystem->SetOrigin(childOrigin);
            system->children.push_back(childSystem);
        }
        else
        {
            DBGWARNING("!! Failed parsing particle child:" << strChild.c_str());
        }
    }
    FOREACH_QDOM_NODE_END;

    return system;
}

template< class T >
void ParticlePrecache::ParseSiblingFunctions(QDomElement *root, QVector<T> &out)
{
    for ( QDomNode c = root->firstChild();
          !c.isNull();
          c = c.nextSibling())
    {
        std::string name = c.toElement().tagName().toStdString();
        IParticleFunction *f = ParticleFunctionFactory::GetInstance()->CreateParticleFunction(name.c_str());
        T concrete = dynamic_cast<T>(f);
        if (concrete != nullptr)
        {
            QDomElement element = c.toElement();

            QDomNodeList list = element.childNodes();
            for ( int i = 0; i < list.length(); i++ )
            {
                QDomElement e = list.at(i).toElement();
                std::string strTag = e.tagName().toStdString();
                std::string strVal = e.text().toStdString();
                const char *tag = strTag.c_str();
                const char *val = strVal.c_str();

                if (!f->ParseFromXML(tag, val, &element))
                {
                    DBGWARNING("!! Unknown particle function parameter:"
                             << tag << "in function:" << name.c_str());
                }
            }

            out.append(concrete);
        }
        else
        {
            DBGWARNING("!! Missing function:" << name.c_str() << "!");
        }
    }
}
