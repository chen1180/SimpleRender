#include "GLwindow.h"
void GLwindow::initializeGL() {
    f = context()->functions();
    f->initializeOpenGLFunctions();

    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_DEPTH_TEST);
    //f->glEnable(GL_CULL_FACE);
    
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "phong.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "phong.frag");
    m_program->link();

    light_program = new QOpenGLShaderProgram(this);
    light_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "flat.vert");
    light_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "flat.frag");
    light_program->link();

    //auto mesh = std::make_unique<MeshTriangle>(QVector3D(0,2,0));
    //mesh->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_triangulated_good.obj");
    //mesh->addTexture("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_texture.png");
    //mesh->material = red_plastic;
    //mesh->setupBuffer(m_program);

    //auto plane = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    //plane->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/plane.obj");
    ////shortbox->material = emerald;
    //plane->setupBuffer(m_program);

    auto cornell = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    cornell->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/cornellbox.obj");
    cornell->setupBuffer(m_program);
    auto sponza = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    sponza.get()->scale = QVector3D(0.1, 0.1, 0.1);
    sponza->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/backpack/backpack.obj");
    sponza->setupBuffer(m_program);

    auto *light = new MeshTriangle();
    light->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/box.obj");
    light->setupBuffer(m_program);

    //scene.Add(std::move(shortbox));
    //scene.Add(std::move(mesh));
    //scene.Add(std::move(plane));
    scene.Add(std::move(cornell));
    scene.Add(std::move(sponza));
    //light
    auto light1 = std::make_unique<Light>(QVector3D(0,3,0),100);
    light1->shape = light;
    light1->ambient = QVector3D(0.2, 0.2, 0.2);
    light1->diffuse = QVector3D(0.8, 0.8, 0.8);
    light1->specular = QVector3D(1.0, 1.0, 1.0);
    scene.Add(std::move(light1));

    auto light2 = std::make_unique<Light>(QVector3D(3, -3, 5), 100);
    light2->shape = light;
    light2->ambient = QVector3D(0.2, 0.2, 0.2);
    light2->diffuse = QVector3D(0.8, 0.8, 0.8);
    light2->specular = QVector3D(1.0, 1.0, 1.0);
    scene.Add(std::move(light2));
}
void GLwindow::paintGL() {

    f->glViewport(0, 0, scene.width,scene.height);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // issue some native OpenGL commands
    QMatrix4x4 view=scene.arcball.transform();

    QMatrix4x4 projection;
    projection.perspective(60.0f, (float)scene.width/scene.height, 0.1f, 1000.0f);
    m_program->bind();

    m_program->setUniformValue("view", view);
    m_program->setUniformValue("projection", projection);
    m_program->setUniformValue("viewPos", scene.arcball.eye());

    for (auto& b : scene.get_objects()) {
        int i = 0;
        for (auto& light : scene.get_lights())
        {
            Light& l = *light;
            m_program->setUniformValue(QString("light[%1].position").arg(i).toStdString().c_str(), l.position);
            m_program->setUniformValue(QString("light[%1].ambient").arg(i).toStdString().c_str(), l.ambient);
            m_program->setUniformValue(QString("light[%1].diffuse").arg(i).toStdString().c_str(), l.diffuse);
            m_program->setUniformValue(QString("light[%1].specular").arg(i).toStdString().c_str(), l.specular);
            i++;
        }

        MeshTriangle& x = dynamic_cast<MeshTriangle&>(*b);
        QMatrix4x4 model;
        model.scale(x.scale);
        //model.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 180));
        model.translate(x.translation);
        m_program->setUniformValue("model", model);
        m_program->setUniformValue("material.ka", x.material.Ka);
        m_program->setUniformValue("material.ks", x.material.Ks);
        m_program->setUniformValue("material.kd", x.material.Kd);
        m_program->setUniformValue("material.shiniess", x.material.Ns);
        //x.texture->bind();
        /*x.vao.bind();
        f->glDrawArrays(GL_TRIANGLES, 0, x.meshes.vertices.size());*/
        //x.texture->release();
        x.render(f,m_program);
    }
    m_program->release();
    light_program->bind();
    for (auto& light : scene.get_lights())
    {
        Light& l = *light;
        light_program->setUniformValue("view", view);
        light_program->setUniformValue("projection", projection);
        QMatrix4x4 model;
        model.translate(l.position);
        model.scale(QVector3D(0.1, 0.1, 0.1));

        light_program->setUniformValue("model", model);
        l.shape->render(f,m_program);

    }
    light_program->release();
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
        //scene.camera.ProcessMouseScroll(delta);
        scene.arcball.zoom(delta*0.01);

    }
    event->accept();
}
void GLwindow::keyPressEvent(QKeyEvent* event) {

}
