#include "GLwindow.h"
void GLwindow::initializeGL() {
    f = context()->functions();
    f->initializeOpenGLFunctions();


    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_DEPTH_TEST);
    
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "phong.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "phong.frag");
    m_program->link();

    light_program = new QOpenGLShaderProgram(this);
    light_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "flat.vert");
    light_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "flat.frag");
    light_program->link();

    auto mesh = std::make_unique<MeshTriangle>(QVector3D(0,0,0));
    mesh->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_triangulated_good.obj");
    mesh->addTexture("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_texture.png");
    mesh->setupBuffer(m_program);
    mesh->diffuse = QVector3D(0.6, 0.6, 0.6);
 /*   auto floor = std::make_unique<MeshTriangle>();
    floor->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/floor.obj");
    floor->setupBuffer(m_program);
    floor->diffuse = QVector3D(0.6, 0.6, 0.6);

    auto left = std::make_unique<MeshTriangle>();
    left->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/left.obj");
    left->setupBuffer(m_program);
    left->diffuse = QVector3D(0.6, 0.1, 0.1);

    auto right = std::make_unique<MeshTriangle>();
    right->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/right.obj");
    right->setupBuffer(m_program);
    right->diffuse = QVector3D(0.1, 0.6, 0.1);

    auto shortbox = std::make_unique<MeshTriangle>();
    shortbox->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/shortbox.obj");
    shortbox->setupBuffer(m_program);
    shortbox->diffuse = QVector3D(0.6, 0.6, 0.6);

    auto tallbox = std::make_unique<MeshTriangle>();
    tallbox->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/tallbox.obj");
    tallbox->setupBuffer(m_program);
    tallbox->diffuse = QVector3D(0.6, 0.6, 0.6);*/

    auto *light = new MeshTriangle();
    light->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/box.obj");
    light->setupBuffer(m_program);
    light->diffuse = QVector3D(0.6, 0.6, 0.6);

   /* scene.Add(std::move(floor));
    scene.Add(std::move(left));
    scene.Add(std::move(right));
    scene.Add(std::move(shortbox));
    scene.Add(std::move(tallbox));*/
    scene.Add(std::move(mesh));
    //light
    auto light1 = std::make_unique<Light>(QVector3D(0,5,0),100);
    light1->shape = light;
    scene.Add(std::move(light1));
}
void GLwindow::paintGL() {

    f->glViewport(0, 0, this->width(), this->height());
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // issue some native OpenGL commands
    QMatrix4x4 view= scene.camera.GetViewMatrix();

    QMatrix4x4 projection;
    projection.perspective(scene.camera.Zoom, 4.0f / 3.0f, 0.1f, 1000.0f);
    m_program->bind();

    m_program->setUniformValue("view", view);
    m_program->setUniformValue("projection", projection);

    m_program->setUniformValue("cameraPos", scene.camera.Position);
    std::cout << scene.camera.Position[0] <<" "<< scene.camera.Position[1] << " " << scene.camera.Position[2] << std::endl;
  
    m_program->setUniformValue("lightColor", QVector3D(1.0, 1.0, 1.0));
    m_program->setUniformValue("ambientColor", QVector3D(1.0, 0.5, 0.31));

    m_program->setUniformValue("specularColor", QVector3D(0.5, 0.5, 0.5));
    for (auto& b : scene.get_objects()) {
        int i = 0;
        for (auto& light : scene.get_lights())
        {
            Light& l = *light;
            QString tStr = QString("lightPos[%1]").arg(i);
            m_program->setUniformValue(tStr.toStdString().c_str(), l.position);
            i++;
        }
        MeshTriangle& x = dynamic_cast<MeshTriangle&>(*b);
        QMatrix4x4 model;
        model.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 180));
        model.translate(x.modelPosition);
        m_program->setUniformValue("model", model);
        m_program->setUniformValue("diffuseColor", x.diffuse);
        //x.texture->bind();
        x.vao.bind();
        f->glDrawArrays(GL_TRIANGLES, 0, x.data.size());
        //x.texture->release();

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
        light_program->setUniformValue("model", model);
        l.shape->vao.bind();
        f->glDrawArrays(GL_TRIANGLES, 0, l.shape->data.size());

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
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();
    if (event->buttons() & Qt::LeftButton) {
        scene.camera.ProcessMouseMovement(deltaX, deltaY,true);
        update();
    }
    lastMousePosition = event->pos();
    event->accept();
}
void GLwindow::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    if (event->orientation() == Qt::Vertical) {
        scene.camera.ProcessMouseScroll(delta);
        update();
    }
    event->accept();
}
void GLwindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key())
    {
    case Qt::Key_A:
        scene.camera.ProcessKeyboard(LEFT, 0.1);
        break;
    case Qt::Key_S:
        scene.camera.ProcessKeyboard(BACKWARD, 0.1);
        break;
    case Qt::Key_D:
        scene.camera.ProcessKeyboard(RIGHT, 0.1);
        break;
    case Qt::Key_W:
        scene.camera.ProcessKeyboard(FORWARD, 0.1);
        break;
    default:
        break;
    }

}
