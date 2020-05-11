#pragma once
#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H
#include<qstring.h>
#include<vector>
#include<qopenglbuffer.h>`
#include<qopenglshaderprogram.h>
#include<qopenglvertexarrayobject.h>
#include"Object.h"
#include"Texture.h"

class MeshTriangle:public Object
{
public:
    MeshTriangle(const QVector3D& pos){
        position = pos;
    }
    MeshTriangle() {
    }
    ~MeshTriangle() {

    }
    void load(const QString& model_path);
    void setupBuffer(QOpenGLShaderProgram* m_program);
    void addTexture(const QString& path);
    void render(QOpenGLShaderProgram* m_program);
    std::vector<float> data;
    Texture* texture;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

};
#endif MESHTRIANGLE_H



