#pragma once
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <qopenglwindow.h>
#include<gl/GL.h>
#include<qopenglfunctions_4_3_core.h>
#include<qopenglfunctions.h>
#include<qopenglshaderprogram.h>
#include<qopenglbuffer.h>
#include<qopenglvertexarrayobject.h>
#include<qopenglframebufferobject.h>
#include<qevent.h>
#include"Texture.h"
#include"MeshTriangle.h"
#include"Scene.h"
#include<QtCore/qt>
#include <QTime>
#include<qpainter.h>
#include"quad.h"
#include"Rasteriser.h"
#include"Raytracer.h"
class GLwindow : public QOpenGLWindow
{
	Q_OBJECT

public:
    GLwindow() {
    }
    void resizeGL(int w, int h);
    void initializeGL();
    void paintGL();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    Rasteriser* render;
    Raytracer* tracer;
    QOpenGLContext* m_context;

    QOpenGLShaderProgram* m_program;
    QOpenGLShaderProgram* light_program;
    QOpenGLShaderProgram* color_program;
    Texture* text;
    unique_ptr<Quad> quad;
    QTime frameTime;
    Scene scene;
    QPoint lastMousePosition;
 

};

#endif GLWINDOW_H
