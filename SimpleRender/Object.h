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
    Material* m;
    QVector3D diffuse;
    QVector3D modelPosition;

};
