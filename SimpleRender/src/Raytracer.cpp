#include "Raytracer.h"
#include<iostream>
Raytracer::Raytracer(Scene* scene) :Renderer(scene) {
}
void Raytracer::init(QOpenGLFunctions_4_3_Core* f) {
    if (initialized)
        return;
    if (scene == nullptr) {
        std::cout << "Error: No Scene Found!\n";
        return;
    }

    quad = make_unique<Quad>();
    samplesCount = 1.0;
    //shader initalization
    pathTraceShader = new QOpenGLShaderProgram();
    pathTraceShader->addShaderFromSourceFile(QOpenGLShader::Compute, "shaders/raytracing_bruteforce.comp");
    pathTraceShader->link();


    pathTraceShaderLowRes = new QOpenGLShaderProgram();
    pathTraceShaderLowRes->addShaderFromSourceFile(QOpenGLShader::Compute, "shaders/raytracing_progressive.comp");
    pathTraceShaderLowRes->link();

    accumShader = new QOpenGLShaderProgram();
    accumShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
    accumShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/quad.frag");
    accumShader->link();

    outputShader = new QOpenGLShaderProgram();
    outputShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/quad.vert");
    outputShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/output.frag");
    outputShader->link();


    pathTraceFBO = new QOpenGLFramebufferObject(scene->width, scene->width, QOpenGLFramebufferObject::NoAttachment);
    pathTraceFBO->bind();
    //process of define an image rather than texture
    f->glGenTextures(1, &pathTraceTexture);
    f->glBindTexture(GL_TEXTURE_2D, pathTraceTexture);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene->width, scene->width);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    //m_fbo->addColorAttachment(scene.width, scene.height, GL_RGBA32F);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTraceTexture, 0);

    accumFBO = new QOpenGLFramebufferObject(scene->width, scene->width, QOpenGLFramebufferObject::NoAttachment);
    accumFBO->bind();
    f->glGenTextures(1, &accumTexture);
    f->glBindTexture(GL_TEXTURE_2D, accumTexture);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene->width, scene->width);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);

    pathTraceFBOLowRes = new QOpenGLFramebufferObject(scene->width, scene->width, QOpenGLFramebufferObject::NoAttachment);
    pathTraceFBOLowRes->bind();
    f->glGenTextures(1, &pathTraceTextureLowRes);
    f->glBindTexture(GL_TEXTURE_2D, pathTraceTextureLowRes);
    f->glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, scene->width, scene->width);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTraceTextureLowRes, 0);

    //create sphere buffer;
    sceneBuffer = new QOpenGLBuffer();
    sceneBuffer->create();
    f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
    f->glBufferData(GL_SHADER_STORAGE_BUFFER, scene->get_spheres().size()*sizeof(Sphere) , &scene->get_spheres()[0], GL_DYNAMIC_COPY);
    //the followng code can modify buffer structure in glsl
    //f->glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(unsigned int), (uint*)((uint)scene->get_spheres().size()));
    //f->glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int), scene->get_spheres().size() * sizeof(Sphere), &scene->get_spheres()[0]);
    f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    initialized = true;
}
void Raytracer::finish() {
    if (!initialized)
        return;
    initialized = false;
    std::cout << "Raytracer finished!\n";
}
void Raytracer::render(QOpenGLFunctions_4_3_Core* f) {
    if (!initialized)
    {
        std::cout << "Raytracer is not initialized\n";
        return;
    }
    QMatrix4x4 view = scene->arcball.transform();
    QMatrix4x4 projection;
    projection.perspective(scene->fov, (float)scene->width / scene->height, 0.1f, 1000.0f);
    //frame
    if (scene->arcball.isMoving == false) {
        pathTraceFBO->bind();
        pathTraceShader->bind();
        pathTraceShader->setUniformValue("cameraPos", scene->arcball.eye());
        pathTraceShader->setUniformValue("cameraLookAt", scene->arcball.dir());
        pathTraceShader->setUniformValue("iTime", scene->frameCount);
        // Compute appropriate invocation dimension.
        int worksizeX = nextPowerOfTwo(scene->width);
        int worksizeY = nextPowerOfTwo(scene->height);
        int workGrouSize[3];
        f->glGetProgramiv(pathTraceShader->programId(), GL_COMPUTE_WORK_GROUP_SIZE, workGrouSize);
        int workGroupSizeX, workGroupSizeY;
        workGroupSizeX = workGrouSize[0];
        workGroupSizeY = workGrouSize[1];
        // Invoke the compute shader.
        f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
        f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sceneBuffer->bufferId());
        f->glBindImageTexture(0, pathTraceTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        f->glBindImageTexture(1, accumTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        f->glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);
        // Reset image binding.
        f->glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        f->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        pathTraceShader->release();

        accumFBO->bind();
        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw screen texture
        accumShader->bind();
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, pathTraceTexture);
        quad->Draw(accumShader);
        accumFBO->bindDefault();

        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw output texture
        outputShader->bind();
        outputShader->setUniformValue("invSamples", 1.0f / samplesCount);
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, accumTexture);
        quad->Draw(outputShader);
        ++samplesCount;
    }
    else {
        pathTraceFBOLowRes->bind();
        pathTraceShaderLowRes->bind();
        pathTraceShaderLowRes->setUniformValue("cameraPos", scene->arcball.eye());
        pathTraceShaderLowRes->setUniformValue("cameraLookAt", scene->arcball.dir());
        pathTraceShaderLowRes->setUniformValue("iTime", scene->frameCount);

        // Compute appropriate invocation dimension.
        int worksizeX = nextPowerOfTwo(scene->width);
        int worksizeY = nextPowerOfTwo(scene->height);
        int workGrouSize[3];
        f->glGetProgramiv(pathTraceShaderLowRes->programId(), GL_COMPUTE_WORK_GROUP_SIZE, workGrouSize);
        int workGroupSizeX, workGroupSizeY;
        workGroupSizeX = workGrouSize[0];
        workGroupSizeY = workGrouSize[1];
        f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->bufferId());
        f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sceneBuffer->bufferId());
        f->glBindImageTexture(0, pathTraceTextureLowRes, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        f->glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);
        // Reset image binding.
        f->glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        f->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        pathTraceShaderLowRes->release();

        accumFBO->bind();
        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw screen texture
        accumShader->bind();
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, pathTraceTextureLowRes);
        quad->Draw(accumShader);
        accumFBO->bindDefault();
        f->glClear(GL_COLOR_BUFFER_BIT);
        //draw screen texture
        outputShader->bind();
        outputShader->setUniformValue("invSamples", 1.0f / samplesCount);
        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, accumTexture);
        quad->Draw(outputShader);
        samplesCount = 1.0;
    }
}
void Raytracer::present() const {}
void Raytracer::update(float secondsElapsed) {}