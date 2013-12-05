#include "materialprecache.h"

#include "base.h"

#include <QPainter>
#include <QtXml>

MaterialPrecache MaterialPrecache::instance;
MaterialPrecache *MaterialPrecache::GetInstance()
{
    return &instance;
}

MaterialPrecache::MaterialPrecache()
    : errorMaterial( nullptr )
{
}

MaterialPrecache::~MaterialPrecache()
{
    delete errorMaterial;

    DESTROY_QHASH(QString, Material*, materials);
}

void MaterialPrecache::PrecacheMaterial(const char *path)
{
    QString shortPath(path);
    shortPath.remove(".xml");

    if (materials.contains(shortPath))
    {
        DBGWARNING("!! Already precached:" << path);
        return;
    }

    std::string fullPath(PATH_MATERIAL_ROOT);
    fullPath += shortPath.toStdString();
    fullPath += ".xml";

    Material *material = ParseMaterial(fullPath.c_str());

    if (material == nullptr)
    {
        DBGWARNING("!! Failed precaching material:" << path);
        return;
    }

    materials.insert(shortPath, material);
}

Material *MaterialPrecache::GetMaterial(const char *path)
{
    if (materials.contains(path))
    {
        return materials[path];
    }

    DBGWARNING("!! Material not precached:" << path);

    if (errorMaterial == nullptr)
    {
        CreateErrorMaterial();
    }

    return errorMaterial;
}

Material *MaterialPrecache::ParseMaterial(const char *path)
{
    QDomElement root;

    if (!OpenXMLFile(path, root))
    {
        return nullptr;
    }

    QImage *texture = new QImage();

    QString texturePath(PATH_MATERIAL_ROOT);
    texturePath += root.firstChildElement("texture").text();

    if (!texture->load(OSLocalPath(texturePath)))
    {
        delete texture;
        DBGWARNING("!! Error precaching texture:" << path
                 << ", for material:" << path);
        return nullptr;
    }

    QPainter::CompositionMode mode = StringToCompositionMode(
                root.firstChildElement("blendmode").text());

    QDomElement eOpacity = root.firstChildElement("opacity");
    QDomElement eTint = root.firstChildElement("tint");
    QDomElement eNoFilter = root.firstChildElement("nofilter");

    float opacity = eOpacity.isNull() ? 1.0f : eOpacity.text().toFloat();
    int flags = Material::MF_NONE;

    if (XMLParseBoolean(eNoFilter))
    {
        flags |= Material::MF_NEVER_FILTER;
    }

    if (eTint.isNull())
    {
        return new Material(texture, opacity, mode, flags);
    }
    else
    {
        QColor tint = XMLParseColor(eTint);
        return new Material(texture, opacity, mode, tint, flags);
    }
}

void MaterialPrecache::CreateErrorMaterial()
{
    QImage *errorTexture = new QImage(64, 64, QImage::Format_RGB32);
    errorTexture->fill(Qt::cyan);

    QPainter painter(errorTexture);

    painter.setBrush(Qt::magenta);
    painter.setPen(Qt::transparent);

    painter.drawRect(0,0,16,16);
    painter.drawRect(0,32,16,16);
    painter.drawRect(16,16,16,16);
    painter.drawRect(16,48,16,16);
    painter.drawRect(32,0,16,16);
    painter.drawRect(32,32,16,16);
    painter.drawRect(48,16,16,16);
    painter.drawRect(48,48,16,16);

    painter.setBrush(Qt::green);
    painter.drawRect(0,0,64,1);

    painter.setBrush(Qt::red);
    painter.drawRect(0,63,64,1);

    painter.end();

    if (errorTexture->isNull())
        DBGWARNING("!! Failed to create error texture");

    errorMaterial = new Material(errorTexture);
}
