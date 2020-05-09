#pragma once
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <qopenglwindow.h>
#include<gl/GL.h>
#include<qopenglfunctions.h>
#include<qopenglshaderprogram.h>
#include<qopenglbuffer.h>`
#include<qopenglvertexarrayobject.h>
#include<qevent.h>
#include"Texture.h"
#include"MeshTriangle.h"
class GLwindow : public QOpenGLWindow
{
	Q_OBJECT

public:
    GLwindow() {
        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        setFormat(format);

    }
    void resizeGL(int w, int h);
    void initializeGL();
    void paintGL();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    QOpenGLContext m_context;
    QOpenGLFramebufferObject* m_fbo;
    QOpenGLFunctions* f;
    QOpenGLShaderProgram* m_program;

    Texture* texture;
    MeshTriangle mesh;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_textAttr;
    GLuint modelUniform;
    GLuint viewUniform;
    GLuint projectionUniform;
    GLuint textureUniform;

    double alpha=25;
    double beta=-25;
    double distance=2.5;
    QPoint lastMousePosition;

};
#endif GLWINDOW_H
