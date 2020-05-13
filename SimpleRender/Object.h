#pragma once
#include<qvector3d.h>
#include<vector>
struct Material
{
    Material()
    {
        name;
        Ns = 0.0f;
        Ni = 0.0f;
        d = 0.0f;
        illum = 0;
        Ka = QVector3D(0.2, 0.2, 0.2);
        Kd = QVector3D(0.8, 0.8, 0.8);
        Ks = QVector3D(1.0, 1.0, 1.0);
        Ns = 0.25 * 128;
    }

    // Material Name
    std::string name;
    // Ambient Color
    QVector3D Ka;
    // Diffuse Color
    QVector3D Kd;
    // Specular Color
    QVector3D Ks;
    // Specular Exponent
    float Ns;
    // Optical Density
    float Ni;
    // Dissolve
    float d;
    // Illumination
    int illum;
    // Ambient Texture Map
    std::string map_Ka;
    // Diffuse Texture Map
    std::string map_Kd;
    // Specular Texture Map
    std::string map_Ks;
    // Specular Hightlight Map
    std::string map_Ns;
    // Alpha Texture Map
    std::string map_d;
    // Bump Map
    std::string map_bump;
};

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    Material material={ };
    QVector3D position;

};
