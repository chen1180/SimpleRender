#include "GLwindow.h"
void GLwindow::initializeGL() {
    QOpenGLFunctions_4_3_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_ALWAYS);
    f->glEnable(GL_TEXTURE_2D);
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

    //auto cornell = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    //cornell->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/cornellbox/cornellbox.obj");
    //cornell->setupBuffer(m_program);

    //auto backpack = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    //backpack->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/backpack/backpack.obj");
    //backpack->setupBuffer(m_program);

    //auto sponza = std::make_unique<MeshTriangle>(QVector3D(0, 0, 0));
    //sponza.get()->scale = QVector3D(0.1, 0.1, 0.1);
    //sponza->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/Sponza/sponza.obj");
    //sponza->setupBuffer(m_program);

    auto *light = new MeshTriangle();
    light->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/box.obj");
    light->setupBuffer(light_program);

    //scene.Add(std::move(shortbox));
    //scene.Add(std::move(mesh));
    //scene.Add(std::move(backpack));
    //scene.Add(std::move(cornell));
    scene.Add(std::move(plane));
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

    //Ray tracing part
    rt_program = new QOpenGLShaderProgram(this);
    rt_program->addShaderFromSourceFile(QOpenGLShader::Compute, "raytracing.comp");
    rt_program->link();

    m_fbo = new QOpenGLFramebufferObject(scene.width, scene.width, QOpenGLFramebufferObject::NoAttachment);


    //texture
    text = new Texture();
    //text->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/Crate/crate_1.jpg");
    text->texture = new QOpenGLTexture(QImage(scene.width, scene.height, QImage::Format_RGBA8888));
    text->texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    text->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    text->id = text->texture->textureId();


    f->glGenTextures(1, &tex);
    f->glBindTexture(GL_TEXTURE_2D, tex);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene.width, scene.height);
   /* f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, tex);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, scene.width, scene.height, 0, GL_RGBA, GL_FLOAT,
        NULL);*/
   
    //quad data
    quad_program = new QOpenGLShaderProgram(this);
    quad_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "quad.vert");
    quad_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "quad.frag");
    quad_program->link();
    quad_program->bind();

    vertex = std::vector<float>{ 
         -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,

        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };
    quadVBO=new QOpenGLBuffer();
    quadVAO=new QOpenGLVertexArrayObject();

    quadVAO->create();
    quadVAO->bind();
    //create buffer (Do not release until VAO is created)
    quadVBO->create();
    quadVBO->bind();
    quadVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    quadVBO->allocate(vertex.data(),vertex.size()* sizeof(float));
    // Create Vertex Array Object
    quad_program->enableAttributeArray(0);
    quad_program->setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(float)*2);
    quadVAO->release();
    quadVBO->release();
    quad_program->release();

}
void GLwindow::paintGL() {
    QOpenGLFunctions_4_3_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    f->glViewport(0, 0, scene.width,scene.height);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    //m_fbo->bind();
    //scene.Render(f);
    //m_fbo->bindDefault();
  
    QMatrix4x4 view = scene.arcball.transform();
    QMatrix4x4 projection;
    projection.perspective(scene.fov, (float)scene.width / scene.height, 0.1f, 1000.0f);
    QMatrix4x4 invViewProj = (projection * view).inverted();
    QVector3D ray00, ray01, ray10, ray11;
    ray00 = (QVector4D(-1, -1, 0, 1) * invViewProj).toVector3DAffine()-scene.arcball.eye();
    ray01 = (QVector4D(-1, 1, 0, 1) * invViewProj).toVector3DAffine() - scene.arcball.eye();
    ray10 = (QVector4D(1, -1, 0, 1) * invViewProj).toVector3DAffine() - scene.arcball.eye();
    ray11 = (QVector4D(1, 1, 0, 1) * invViewProj).toVector3DAffine() - scene.arcball.eye();
    //m_fbo->bind();
    rt_program->bind();
    rt_program->setUniformValue("eye", scene.arcball.eye());
    rt_program->setUniformValue("ray00", ray00);
    rt_program->setUniformValue("ray01", ray01);
    rt_program->setUniformValue("ray10", ray10);
    rt_program->setUniformValue("ray11", ray11);

    // Compute appropriate invocation dimension.
    int worksizeX = nextPowerOfTwo(scene.width);
    int worksizeY =nextPowerOfTwo(scene.height);
    int* workGrouSize;
    f->glGetProgramiv(rt_program->programId(), GL_COMPUTE_WORK_GROUP_SIZE, workGrouSize);
    int workGroupSizeX, workGroupSizeY;
    workGroupSizeX = workGrouSize[0];
    workGroupSizeY = workGrouSize[1];
    // Invoke the compute shader.
    //text->bind();
    //rt_program->setUniformValue(rt_program->uniformLocation("text"), text->id);
    //std::cout << text->id<<' '<< rt_program->uniformLocation("text");
    f->glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    f->glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);
    // Reset image binding.
    f->glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    f->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    rt_program->release();
    //m_fbo->bindDefault();

    //draw screen texture
    //f->glClear(GL_COLOR_BUFFER_BIT);

    quad_program->bind();
    f-> glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, tex);
    quadVAO->bind();
    f->glDrawArrays(GL_TRIANGLES, 0,6);
    quadVAO->release();
    quad_program->release();
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
