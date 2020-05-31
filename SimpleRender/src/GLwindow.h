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
struct Sphere
{
    // sphere properties
    QVector3D center;

    float radius;
    // material
    int   materialType;
    QVector3D  albedo;
    float fuzz;
    float refractionIndex;
};
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
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    Rasteriser* render;
    QOpenGLContext* m_context;
    QOpenGLFramebufferObject* m_fbo,*accum_fbo,*progressive_fbo;

    QOpenGLShaderProgram* m_program;
    QOpenGLShaderProgram* light_program;
    QOpenGLShaderProgram* output_program;
    QOpenGLShaderProgram* progressive_program;
    QOpenGLBuffer *sceneBuffer;
    Texture* text;
    float samples = 1.0;
    QOpenGLShaderProgram* quad_program ;
    QOpenGLShaderProgram* rt_program;
    GLuint tex,accum_tex,progressive_tex;
    unique_ptr<Quad> quad;

    QTime frameTime;
    Scene scene;
    double alpha=25;
    double beta=-25;
    double distance=2.5;
    QPoint lastMousePosition;
    Sphere sceneList[84] = {
        Sphere{QVector3D{0.000000, -1000.000000, 0.000000}, 1000.000000,0, QVector3D{0.500000, 0.500000, 0.500000},  1.000000, 1.000000},
        Sphere{QVector3D{-7.995381, 0.200000, -7.478668}, 0.200000, 0, QVector3D{0.380012, 0.506085, 0.762437}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.696819, 0.200000, -5.468978}, 0.200000, 0, QVector3D{0.596282, 0.140784, 0.017972}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.824804, 0.200000, -3.120637}, 0.200000, 0, QVector3D{0.288507, 0.465652, 0.665070}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.132909, 0.200000, -1.701323}, 0.200000, 0, QVector3D{0.101047, 0.293493, 0.813446}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.569523, 0.200000, 0.494554}, 0.200000, 0, QVector3D{0.365924, 0.221622, 0.058332}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.730332, 0.200000, 2.358976}, 0.200000, 0, QVector3D{0.051231, 0.430547, 0.454086}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.892865, 0.200000, 4.753728}, 0.200000, 1, QVector3D{0.826684, 0.820511, 0.908836}, 0.389611, 1.000000},
        Sphere{QVector3D{-7.656691, 0.200000, 6.888913}, 0.200000, 0, QVector3D{0.346542, 0.225385, 0.180132}, 1.000000, 1.000000},
        Sphere{QVector3D{-7.217835, 0.200000, 8.203466}, 0.200000, 1, QVector3D{0.600463, 0.582386, 0.608277}, 0.427369, 1.000000},
        Sphere{QVector3D{-5.115232, 0.200000, -7.980404}, 0.200000, 0, QVector3D{0.256969, 0.138639, 0.080293}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.323222, 0.200000, -5.113037}, 0.200000, 0, QVector3D{0.193093, 0.510542, 0.613362}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.410681, 0.200000, -3.527741}, 0.200000, 0, QVector3D{0.352200, 0.191551, 0.115972}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.460670, 0.200000, -1.166543}, 0.200000, 0, QVector3D{0.029486, 0.249874, 0.077989}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.457659, 0.200000, 0.363870}, 0.200000, 0, QVector3D{0.395713, 0.762043, 0.108515}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.798715, 0.200000, 2.161684}, 0.200000, 2, QVector3D{0.000000, 0.000000, 0.000000}, 1.000000, 1.500000},
        Sphere{QVector3D{-5.116586, 0.200000, 4.470188}, 0.200000, 0, QVector3D{0.059444, 0.404603, 0.171767}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.273591, 0.200000, 6.795187}, 0.200000, 0, QVector3D{0.499454, 0.131330, 0.158348}, 1.000000, 1.000000},
        Sphere{QVector3D{-5.120286, 0.200000, 8.731398}, 0.200000, 0, QVector3D{0.267365, 0.136024, 0.300483}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.601565, 0.200000, -7.895600}, 0.200000, 0, QVector3D{0.027752, 0.155209, 0.330428}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.735860, 0.200000, -5.163056}, 0.200000, 1, QVector3D{0.576768, 0.884712, 0.993335}, 0.359385, 1.000000},
        Sphere{QVector3D{-3.481116, 0.200000, -3.794556}, 0.200000, 0, QVector3D{0.405104, 0.066436, 0.009339}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.866858, 0.200000, -1.465965}, 0.200000, 0, QVector3D{0.027570, 0.021652, 0.252798}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.168870, 0.200000, 0.553099}, 0.200000, 0, QVector3D{0.421992, 0.107577, 0.177504}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.428552, 0.200000, 2.627547}, 0.200000, 1, QVector3D{0.974029, 0.653443, 0.571877}, 0.312780, 1.000000},
        Sphere{QVector3D{-3.771736, 0.200000, 4.324785}, 0.200000, 0, QVector3D{0.685957, 0.000043, 0.181270}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.768522, 0.200000, 6.384588}, 0.200000, 0, QVector3D{0.025972, 0.082246, 0.138765}, 1.000000, 1.000000},
        Sphere{QVector3D{-3.286992, 0.200000, 8.441148}, 0.200000, 0, QVector3D{0.186577, 0.560376, 0.367045}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.552127, 0.200000, -7.728200}, 0.200000, 0, QVector3D{0.202998, 0.002459, 0.015350}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.360796, 0.200000, -5.346098}, 0.200000, 0, QVector3D{0.690820, 0.028470, 0.179907}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.287209, 0.200000, -3.735321}, 0.200000, 0, QVector3D{0.345974, 0.672353, 0.450180}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.344859, 0.200000, -1.726654}, 0.200000, 0, QVector3D{0.209209, 0.431116, 0.164732}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.974774, 0.200000, 0.183260}, 0.200000, 0, QVector3D{0.006736, 0.675637, 0.622067}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.542872, 0.200000, 2.067868}, 0.200000, 0, QVector3D{0.192247, 0.016661, 0.010109}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.743856, 0.200000, 4.752810}, 0.200000, 0, QVector3D{0.295270, 0.108339, 0.276513}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.955621, 0.200000, 6.493702}, 0.200000, 0, QVector3D{0.270527, 0.270494, 0.202029}, 1.000000, 1.000000},
        Sphere{QVector3D{-1.350449, 0.200000, 8.068503}, 0.200000, 1, QVector3D{0.646942, 0.501660, 0.573693}, 0.346551, 1.000000},
        Sphere{QVector3D{0.706123, 0.200000, -7.116040}, 0.200000, 0, QVector3D{0.027695, 0.029917, 0.235781}, 1.000000, 1.000000},
        Sphere{QVector3D{0.897766, 0.200000, -5.938681}, 0.200000, 0, QVector3D{0.114934, 0.046258, 0.039647}, 1.000000, 1.000000},
        Sphere{QVector3D{0.744113, 0.200000, -3.402960}, 0.200000, 0, QVector3D{0.513631, 0.335578, 0.204787}, 1.000000, 1.000000},
        Sphere{QVector3D{0.867750, 0.200000, -1.311908}, 0.200000, 0, QVector3D{0.400246, 0.000956, 0.040513}, 1.000000, 1.000000},
        Sphere{QVector3D{0.082480, 0.200000, 0.838206}, 0.200000, 0, QVector3D{0.594141, 0.215068, 0.025718}, 1.000000, 1.000000},
        Sphere{QVector3D{0.649692, 0.200000, 2.525103}, 0.200000, 1, QVector3D{0.602157, 0.797249, 0.614694}, 0.341860, 1.000000},
        Sphere{QVector3D{0.378574, 0.200000, 4.055579}, 0.200000, 0, QVector3D{0.005086, 0.003349, 0.064403}, 1.000000, 1.000000},
        Sphere{QVector3D{0.425844, 0.200000, 6.098526}, 0.200000, 0, QVector3D{0.266812, 0.016602, 0.000853}, 1.000000, 1.000000},
        Sphere{QVector3D{0.261365, 0.200000, 8.661150}, 0.200000, 0, QVector3D{0.150201, 0.007353, 0.152506}, 1.000000, 1.000000},
        Sphere{QVector3D{2.814218, 0.200000, -7.751227}, 0.200000, 1, QVector3D{0.570094, 0.610319, 0.584192}, 0.018611, 1.000000},
        Sphere{QVector3D{2.050073, 0.200000, -5.731364}, 0.200000, 0, QVector3D{0.109886, 0.029498, 0.303265}, 1.000000, 1.000000},
        Sphere{QVector3D{2.020130, 0.200000, -3.472627}, 0.200000, 0, QVector3D{0.216908, 0.216448, 0.221775}, 1.000000, 1.000000},
        Sphere{QVector3D{2.884277, 0.200000, -1.232662}, 0.200000, 0, QVector3D{0.483428, 0.027275, 0.113898}, 1.000000, 1.000000},
        Sphere{QVector3D{2.644454, 0.200000, 0.596324}, 0.200000, 0, QVector3D{0.005872, 0.860718, 0.561933}, 1.000000, 1.000000},
        Sphere{QVector3D{2.194283, 0.200000, 2.880603}, 0.200000, 0, QVector3D{0.452710, 0.824152, 0.045179}, 1.000000, 1.000000},
        Sphere{QVector3D{2.281000, 0.200000, 4.094307}, 0.200000, 0, QVector3D{0.002091, 0.145849, 0.032535}, 1.000000, 1.000000},
        Sphere{QVector3D{2.080841, 0.200000, 6.716384}, 0.200000, 0, QVector3D{0.468539, 0.032772, 0.018071}, 1.000000, 1.000000},
        Sphere{QVector3D{2.287131, 0.200000, 8.583242}, 0.200000, 2, QVector3D{0.000000, 0.000000, 0.000000}, 1.000000, 1.500000},
        Sphere{QVector3D{4.329136, 0.200000, -7.497218}, 0.200000, 0, QVector3D{0.030865, 0.071452, 0.016051}, 1.000000, 1.000000},
        Sphere{QVector3D{4.502115, 0.200000, -5.941060}, 0.200000, 2, QVector3D{0.000000, 0.000000, 0.000000}, 1.000000, 1.500000},
        Sphere{QVector3D{4.750631, 0.200000, -3.836759}, 0.200000, 0, QVector3D{0.702578, 0.084798, 0.141374}, 1.000000, 1.000000},
        Sphere{QVector3D{4.082084, 0.200000, -1.180746}, 0.200000, 0, QVector3D{0.043052, 0.793077, 0.018707}, 1.000000, 1.000000},
        Sphere{QVector3D{4.429173, 0.200000, 2.069721}, 0.200000, 0, QVector3D{0.179009, 0.147750, 0.617371}, 1.000000, 1.000000},
        Sphere{QVector3D{4.277152, 0.200000, 4.297482}, 0.200000, 0, QVector3D{0.422693, 0.011222, 0.211945}, 1.000000, 1.000000},
        Sphere{QVector3D{4.012743, 0.200000, 6.225072}, 0.200000, 0, QVector3D{0.986275, 0.073358, 0.133628}, 1.000000, 1.000000},
        Sphere{QVector3D{4.047066, 0.200000, 8.419360}, 0.200000, 1, QVector3D{0.878749, 0.677170, 0.684995}, 0.243932, 1.000000},
        Sphere{QVector3D{6.441846, 0.200000, -7.700798}, 0.200000, 0, QVector3D{0.309255, 0.342524, 0.489512}, 1.000000, 1.000000},
        Sphere{QVector3D{6.047810, 0.200000, -5.519369}, 0.200000, 0, QVector3D{0.532361, 0.008200, 0.077522}, 1.000000, 1.000000},
        Sphere{QVector3D{6.779211, 0.200000, -3.740542}, 0.200000, 0, QVector3D{0.161234, 0.539314, 0.016667}, 1.000000, 1.000000},
        Sphere{QVector3D{6.430776, 0.200000, -1.332107}, 0.200000, 0, QVector3D{0.641951, 0.661402, 0.326114}, 1.000000, 1.000000},
        Sphere{QVector3D{6.476387, 0.200000, 0.329973}, 0.200000, 0, QVector3D{0.033000, 0.648388, 0.166911}, 1.000000, 1.000000},
        Sphere{QVector3D{6.568686, 0.200000, 2.116949}, 0.200000, 0, QVector3D{0.590952, 0.072292, 0.125672}, 1.000000, 1.000000},
        Sphere{QVector3D{6.371189, 0.200000, 4.609841}, 0.200000, 1, QVector3D{0.870345, 0.753830, 0.933118}, 0.233489, 1.000000},
        Sphere{QVector3D{6.011877, 0.200000, 6.569579}, 0.200000, 0, QVector3D{0.044868, 0.651697, 0.086779}, 1.000000, 1.000000},
        Sphere{QVector3D{6.096087, 0.200000, 8.892333}, 0.200000, 0, QVector3D{0.588587, 0.078723, 0.044928}, 1.000000, 1.000000},
        Sphere{QVector3D{8.185763, 0.200000, -7.191109}, 0.200000, 1, QVector3D{0.989702, 0.886784, 0.540759}, 0.104229, 1.000000},
        Sphere{QVector3D{8.411960, 0.200000, -5.285309}, 0.200000, 0, QVector3D{0.139604, 0.022029, 0.461688}, 1.000000, 1.000000},
        Sphere{QVector3D{8.047109, 0.200000, -3.427552}, 0.200000, 1, QVector3D{0.815002, 0.631228, 0.806757}, 0.150782, 1.000000},
        Sphere{QVector3D{8.119639, 0.200000, -1.652587}, 0.200000, 0, QVector3D{0.177852, 0.429797, 0.042251}, 1.000000, 1.000000},
        Sphere{QVector3D{8.818120, 0.200000, 0.401292}, 0.200000, 0, QVector3D{0.065416, 0.087694, 0.040518}, 1.000000, 1.000000},
        Sphere{QVector3D{8.754155, 0.200000, 2.152549}, 0.200000, 0, QVector3D{0.230659, 0.035665, 0.435895}, 1.000000, 1.000000},
        Sphere{QVector3D{8.595298, 0.200000, 4.802001}, 0.200000, 0, QVector3D{0.188493, 0.184933, 0.040215}, 1.000000, 1.000000},
        Sphere{QVector3D{8.036216, 0.200000, 6.739752}, 0.200000, 0, QVector3D{0.023192, 0.364636, 0.464844}, 1.000000, 1.000000},
        Sphere{QVector3D{8.256561, 0.200000, 8.129115}, 0.200000, 0, QVector3D{0.002612, 0.598319, 0.435378}, 1.000000, 1.000000},
        Sphere{QVector3D{0.000000, 1.000000, 0.000000}, 1.000000, 2, QVector3D{0.000000, 0.000000, 0.000000}, 1.000000, 1.500000},
        Sphere{QVector3D{-4.000000, 1.000000, 0.000000}, 1.000000, 0, QVector3D{0.400000, 0.200000, 0.100000}, 1.000000, 1.000000},
        Sphere{QVector3D{4.000000, 1.000000, 0.000000}, 1.000000, 1, QVector3D{0.700000, 0.600000, 0.500000}, 0.000000, 1.000000}
    } ;
    uint scene_size= sizeof(sceneList) / sizeof(Sphere);

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
