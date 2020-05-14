#include "GLwindow.h"
void GLwindow::initializeGL() {
    f = context()->functions();
    f->initializeOpenGLFunctions();

    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
    f->glCullFace(GL_BACK);
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "phong.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "phong.frag");
    m_program->link();

    light_program = new QOpenGLShaderProgram(this);
    light_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "flat.vert");
    light_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "flat.frag");
    light_program->link();

    auto plane = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    plane->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/plane.obj");
    plane->setupBuffer(m_program);

    auto cornell = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    cornell->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/cornellbox.obj");
    cornell->setupBuffer(m_program);

    auto backpack = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    backpack->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/backpack/backpack.obj");
    backpack->setupBuffer(m_program);

    //auto sponza = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    //sponza.get()->scale = QVector3D(0.1, 0.1, 0.1);
    //sponza->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/Sponza/sponza.obj");
    //sponza->setupBuffer(m_program);

    auto *light = new MeshTriangle();
    light->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/box.obj");
    light->setupBuffer(light_program);

    //scene.Add(std::move(shortbox));
    //scene.Add(std::move(mesh));
    scene.Add(std::move(backpack));
    scene.Add(std::move(cornell));
    //scene.Add(std::move(sponza));
    //light
    auto light1 = std::make_unique<Light>(QVector3D(0,3,0),100);
    light1->shape = light;
    light1->ambient = QVector3D(0.05, 0.05, 0.05);
    light1->diffuse = QVector3D(0.8, 0.8, 0.8);
    light1->specular = QVector3D(1.0, 1.0, 1.0);
    scene.Add(std::move(light1));

    auto light2 = std::make_unique<Light>(QVector3D(3, -3, 5), 100);
    light2->shape = light;
    light2->ambient = QVector3D(0.05, 0.05, 0.05);
    light2->diffuse = QVector3D(0.8, 0.8, 0.8);
    light2->specular = QVector3D(1.0, 1.0, 1.0);
    scene.Add(std::move(light2));
}
void GLwindow::paintGL() {
    f->glViewport(0, 0, scene.width,scene.height);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Render(f);
    // animate continuously: schedule an update
    update();

   
}
void GLwindow::resizeGL(int w, int h) {
}
void GLwindow::mousePressEvent(QMouseEvent* event)
{
    lastMousePosition = event->pos();
    event->accept();
}
void GLwindow::mouseMoveEvent(QMouseEvent* event)
{
    QVector2D cur_mouse = QVector2D(event->x() * 2.0 / scene.width - 1.0, 1.0 - event->y() * 2.0 / scene.height);
    if (event->buttons() & Qt::LeftButton) {
        scene.arcball.rotate(scene.prev_mouse,cur_mouse);
    }
    if (event->buttons() & Qt::RightButton) {
        scene.arcball.pan(cur_mouse-scene.prev_mouse);
    }

    update();
    scene.prev_mouse = cur_mouse;
    event->accept();
}
void GLwindow::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    if (event->orientation() == Qt::Vertical) {
        scene.arcball.zoom(delta*0.01);

    }
    event->accept();
}
void GLwindow::keyPressEvent(QKeyEvent* event) {

}
