#ifndef MATERIALPRECACHE_H
#define MATERIALPRECACHE_H


#include <QHash>
#include <QImage>

class Material;

class MaterialPrecache
{
    MaterialPrecache();
    ~MaterialPrecache();
    static MaterialPrecache instance;
public:

    static MaterialPrecache *GetInstance();

    void PrecacheMaterial(const char *path);

    Material *GetMaterial(const char *path);

private:
    Material *ParseMaterial(const char *path);
    void CreateErrorMaterial();

    QHash<QString, Material*> materials;

    Material *errorMaterial;
};

#endif // MATERIALPRECACHE_H
