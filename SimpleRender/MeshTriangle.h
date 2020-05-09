#pragma once
#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H
#include<qstring.h>
#include<vector>
#include<qopenglbuffer.h>`
#include<qopenglshaderprogram.h>
#include<qopenglvertexarrayobject.h>
#include"Object.h"
class MeshTriangle:public Object
{
public:
    MeshTriangle() {

    }
    ~MeshTriangle() {

    }
    void load(const QString& model_path);
    void setupBuffer(QOpenGLShaderProgram* m_program);
    void render(QOpenGLShaderProgram* m_program);
    std::vector<float> data;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

};
#endif MESHTRIANGLE_H



