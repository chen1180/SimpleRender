#pragma once
#include<qvector3d.h>
struct Material
{
    QVector3D diffuse;
QVector3D specular;
QVector3D ambient;
};
class Object
{
public:
    Object() {}
    virtual ~Object() {}
    virtual void render();
    Material* m;
};
