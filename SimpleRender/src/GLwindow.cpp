#include "GLwindow.h"
#include<qsize.h>
#define RAY_TRACING true
void GLwindow::initializeGL() {
    QOpenGLFunctions_4_3_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_ALWAYS);
    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_CULL_FACE);
    f->glCullFace(GL_BACK);
    setWidth(scene.width);
    setHeight(scene.height);
    //frame
    frameTime.start();
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/phong.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/phong.frag");
    m_program->link();

    light_program = new QOpenGLShaderProgram(this);
    light_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/flat.vert");
    light_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/flat.frag");
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
    
    render = new Rasteriser(&scene);
    render->init();
    //Ray tracing part
    rt_program = new QOpenGLShaderProgram(this);
    if (RAY_TRACING)
     rt_program->addShaderFromSourceFile(QOpenGLShader::Compute, "shaders/raytracing_bruteforce.comp");
    else
     rt_program->addShaderFromSourceFile(QOpenGLShader::Compute, "shaders/raytracing.comp");
    rt_program->link();

    progressive_program = new QOpenGLShaderProgram(this);
    progressive_program->addShaderFromSourceFile(QOpenGLShader::Compute, "shaders/raytracing_progressive.comp");
    progressive_program->link();
    //texture
    text = new Texture();
    //text->load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/Crate/crate_1.jpg");
    text->texture = new QOpenGLTexture(QImage(scene.width, scene.height, QImage::Format_RGBA8888));
    text->texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    text->texture->setMagnificationFilter(QOpenGLTexture::Linear);
    text->id = text->texture->textureId();

    m_fbo = new QOpenGLFramebufferObject(scene.width, scene.width, QOpenGLFramebufferObject::NoAttachment);
    m_fbo->bind();
    //process of define an image rather than texture
    f->glGenTextures(1, &tex);
    f->glBindTexture(GL_TEXTURE_2D, tex);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene.width, scene.height);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    //m_fbo->addColorAttachment(scene.width, scene.height, GL_RGBA32F);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    accum_fbo = new QOpenGLFramebufferObject(scene.width, scene.width, QOpenGLFramebufferObject::NoAttachment);
    accum_fbo->bind();
    f->glGenTextures(1, &accum_tex);
    f->glBindTexture(GL_TEXTURE_2D, accum_tex);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene.width, scene.height);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accum_tex, 0);

    progressive_fbo = new QOpenGLFramebufferObject(scene.width, scene.width, QOpenGLFramebufferObject::NoAttachment);
    progressive_fbo->bind();
    f->glGenTextures(1, &progressive_tex);
    f->glBindTexture(GL_TEXTURE_2D, progressive_tex);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene.width, scene.height);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    //quad data
    quad_program = new QOpenGLShaderProgram(this);
    quad_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
    quad_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/quad.frag");
    quad_program->link();
    quad_program->bind();

    output_program = new QOpenGLShaderProgram(this);
    output_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
    output_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/output.frag");
    output_program->link();

    quad = make_unique<Quad>();
    //create sphere buffer;
    sceneBuffer= new QOpenGLBuffer();
    sceneBuffer->create();
    f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
    f->glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(sceneList)+ sizeof(scene_size),NULL, GL_DYNAMIC_COPY);
    f->glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(unsigned int),&scene_size);
    f->glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int), sizeof(sceneList), &sceneList);
    //f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sceneBuffer->bufferId());
    f->glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
    //f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
    //Sphere* ptr = (Sphere*)f->glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    //memcpy(ptr, &sceneList, sizeof(sceneList));
    //f->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    //GLvoid* p = f->glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    //memcpy(p, &sceneList, sizeof(sceneList));
    //f->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}
void GLwindow::paintGL() {
    QOpenGLFunctions_4_3_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    QPainter painter(this);
    painter.beginNativePainting();
    f->glViewport(0, 0, scene.width,scene.height);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    QMatrix4x4 view = scene.arcball.transform();
    QMatrix4x4 projection;
    projection.perspective(scene.fov, (float)scene.width / scene.height, 0.1f, 1000.0f);
    QMatrix4x4 invViewProj = (projection * view).inverted();
    QVector3D ray00, ray01, ray10, ray11;
    ray00 = (invViewProj*QVector4D(-1, -1, 0, 1) ).toVector3DAffine()-scene.arcball.eye();
    ray01 = (invViewProj * QVector4D(-1, 1, 0, 1) ).toVector3DAffine() - scene.arcball.eye();
    ray10 = (invViewProj * QVector4D(1, -1, 0, 1) ).toVector3DAffine() - scene.arcball.eye();
    ray11 = (invViewProj * QVector4D(1, 1, 0, 1) ).toVector3DAffine() - scene.arcball.eye();

    //frame
    if (scene.arcball.isMoving == false) {
        m_fbo->bind();
        rt_program->bind();
        if (RAY_TRACING) {
            rt_program->setUniformValue("cameraPos", scene.arcball.eye());
            rt_program->setUniformValue("cameraLookAt", scene.arcball.dir());
            rt_program->setUniformValue("iTime", scene.frameCount);
            //rt_program->setUniformValue("accumTexture",accum_tex);
        }
        else
        {
            rt_program->setUniformValue("eye", scene.arcball.eye());
            rt_program->setUniformValue("ray00", ray00);
            rt_program->setUniformValue("ray01", ray01);
            rt_program->setUniformValue("ray10", ray10);
            rt_program->setUniformValue("ray11", ray11);
            rt_program->setUniformValue("time", scene.frameCount);
        }
        // Compute appropriate invocation dimension.
        int worksizeX = nextPowerOfTwo(scene.width);
        int worksizeY = nextPowerOfTwo(scene.height);
        int workGrouSize[3];
        f->glGetProgramiv(rt_program->programId(), GL_COMPUTE_WORK_GROUP_SIZE, workGrouSize);
        int workGroupSizeX, workGroupSizeY;
        workGroupSizeX = workGrouSize[0];
        workGroupSizeY = workGrouSize[1];
        // Invoke the compute shader.
        //text->bind();
        //rt_program->setUniformValue(rt_program->uniformLocation("text"), text->id);
        //std::cout << text->id<<' '<< rt_program->uniformLocation("text");
        f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
        f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sceneBuffer->bufferId());
        f->glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        f->glBindImageTexture(1, accum_tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        f->glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);
        // Reset image binding.
        f->glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        f->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        rt_program->release();
        m_fbo->bindDefault();

        f->glClear(GL_COLOR_BUFFER_BIT);
        accum_fbo->bind();
        //draw screen texture
        quad_program->bind();
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, tex);
        quad->Draw(quad_program);

        accum_fbo->bindDefault();
        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw output texture
        output_program->bind();
        output_program->setUniformValue("invSamples", 1.0f / samples);
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, accum_tex);
        quad->Draw(output_program);
        ++samples;
    }
    else {
        progressive_fbo->bind();
        progressive_program->bind();
        progressive_program->setUniformValue("cameraPos", scene.arcball.eye());
        progressive_program->setUniformValue("cameraLookAt", scene.arcball.dir());
        progressive_program->setUniformValue("iTime", scene.frameCount);

        // Compute appropriate invocation dimension.
        int worksizeX = nextPowerOfTwo(scene.width);
        int worksizeY = nextPowerOfTwo(scene.height);
        int workGrouSize[3];
        f->glGetProgramiv(rt_program->programId(), GL_COMPUTE_WORK_GROUP_SIZE, workGrouSize);
        int workGroupSizeX, workGroupSizeY;
        workGroupSizeX = workGrouSize[0];
        workGroupSizeY = workGrouSize[1];
        f->glBindImageTexture(0, progressive_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        f->glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);
        // Reset image binding.
        f->glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        f->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        progressive_program->release();
        f->glClear(GL_COLOR_BUFFER_BIT);
        accum_fbo->bind();
        //draw screen texture
        quad_program->bind();
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, progressive_tex);
        quad->Draw(quad_program);
        accum_fbo->bindDefault();
        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw screen texture
        output_program->bind();
        output_program->setUniformValue("invSamples", 1.0f / samples);
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, accum_tex);
        quad->Draw(output_program);
        samples = 1.0;
    }
    scene.frameCount++;

    painter.endNativePainting();
    if (frameTime.elapsed() >= 1000)
    {
        float fps = scene.frameCount / ((double)frameTime.elapsed() / 1000.0);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Helvetica", 12));
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.drawText(0,0,scene.width,scene.height, Qt::AlignTop|| Qt::AlignLeft, QString::fromStdString("FPS: "+std::to_string(static_cast<int>(fps)) + "\nSamples: "+ std::to_string(samples)));
    }
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
        scene.arcball.isMoving = true;
    }
    if (event->buttons() & Qt::RightButton) {
        scene.arcball.pan(cur_mouse-scene.prev_mouse);
        scene.arcball.isMoving = true;
    }

    update();
    scene.prev_mouse = cur_mouse;
    event->accept();
}
void GLwindow::mouseReleaseEvent(QMouseEvent* event) {
    if (scene.arcball.isMoving)
        scene.arcball.isMoving = false;

    update();
    event->accept();
}
void GLwindow::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    if (event->orientation() == Qt::Vertical) {
        scene.arcball.zoom(delta*0.01);
        scene.arcball.isMoving = true;
    }

    event->accept();
}
void GLwindow::keyPressEvent(QKeyEvent* event) {

}
