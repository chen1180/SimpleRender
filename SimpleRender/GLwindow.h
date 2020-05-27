#pragma once
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <qopenglwindow.h>
#include<gl/GL.h>
#include<qopenglfunctions_4_3_core.h>
#include<qopenglfunctions.h>
#include<qopenglshaderprogram.h>
#include<qopenglbuffer.h>`
#include<qopenglvertexarrayobject.h>
#include<qopenglframebufferobject.h>
#include<qevent.h>
#include"Texture.h"
#include"MeshTriangle.h"
#include"Scene.h"
#include<QtCore/qt>
class GLwindow : public QOpenGLWindow
{
	Q_OBJECT

public:
    GLwindow() {
        //// Tell Qt we will use OpenGL for this window
        //setSurfaceType(OpenGLSurface);
        //// Specify the format and create platform-specific surface
        //QSurfaceFormat format;
        //format.setDepthBufferSize(24);
        //format.setMajorVersion(4);
        //format.setMinorVersion(3);
        //format.setSamples(4);
        //format.setProfile(QSurfaceFormat::CoreProfile);
        //setFormat(format);
        //create();

        //// Create an OpenGL context
        //m_context = new QOpenGLContext;
        //m_context->setFormat(format);
        //m_context->create();
        //// Make the context current on this window
        //m_context->makeCurrent(this);
        //// Obtain a functions object and resolve all entry points
        // // m_funcs is declared as: QOpenGLFunctions_4_3_Core* m_funcs
  

        //QOpenGLFunctions_4_3_Core* f =  m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();
        ////std::cout <<f-> glGetString(GL_VERSION);
        ////std::cout << f->glGetString(GL_SHADING_LANGUAGE_VERSION);
        //if (!f) {
        //    qWarning("Could not obtain OpenGL versions object" );
        //    exit(1);
        //}
        //f->initializeOpenGLFunctions();


    }
    void resizeGL(int w, int h);
    void initializeGL();
    void paintGL();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    QOpenGLContext* m_context;
    QOpenGLFramebufferObject* m_fbo;

    QOpenGLShaderProgram* m_program;
    QOpenGLShaderProgram* light_program;
    QOpenGLBuffer* quadVBO;
    QOpenGLVertexArrayObject* quadVAO ;
    Texture* text;
    QOpenGLShaderProgram* quad_program ;
    QOpenGLShaderProgram* rt_program;
    GLuint tex;
    std::vector<float> vertex;

    Scene scene;
    double alpha=25;
    double beta=-25;
    double distance=2.5;
    QPoint lastMousePosition;

};
static int nextPowerOfTwo(int x) {
    x--;
    x |= x >> 1; // handle 2 bit numbers
    x |= x >> 2; // handle 4 bit numbers
    x |= x >> 4; // handle 8 bit numbers
    x |= x >> 8; // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
    return x;
}
#endif GLWINDOW_H
