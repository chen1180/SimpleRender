#pragma once
#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H
#include<qstring.h>
#include<vector>
#include"Object.h"
class MeshTriangle:public Object
{
public:
    MeshTriangle() {

    }
    ~MeshTriangle() {

    }
    void load(const QString& model_path);
    void render()
    std::vector<float> data;

};
#endif MESHTRIANGLE_H



