#pragma once
#include"global.h"
class Quad:protected QOpenGLFunctions_4_3_Core
{
public:
    Quad();
    void Draw(QOpenGLShaderProgram*);
private:
    GLuint vao, vbo;
};

