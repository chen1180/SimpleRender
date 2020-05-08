#include "GLwindow.h"
#include<gl/GL.h>
#include<qopenglfunctions.h>

#include"OBJ_Loader.h"
void GLwindow::initializeGL() {
    f = context()->functions();
    f->initializeOpenGLFunctions();
   
    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_DEPTH_TEST);
    m_program = new QOpenGLShaderProgram(this);

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "flat.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "flat.frag");
    m_program->link();
    m_posAttr = m_program->attributeLocation("aPos");
    m_colAttr = m_program->attributeLocation("aColor");
    m_textAttr = m_program->attributeLocation("aTexCoord");
    projectionUniform = m_program->uniformLocation("projection");
    viewUniform = m_program->uniformLocation("view");
    modelUniform = m_program->uniformLocation("model");
    textureUniform = m_program->uniformLocation("text");
    //load model to triangles

    objl::Loader Loader;

    // Load .obj File
    bool loadout = Loader.LoadFile("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_triangulated_good.obj");
    if (loadout == true) {
        std::cout << "Model successfully loaded!" << std::endl;
        for (auto mesh : Loader.LoadedMeshes)
        {
            for (int i = 0;i < mesh.Vertices.size();i += 3)
            {
                Triangle* t = new Triangle();
                for (int j = 0;j < 3;j++)
                {
                    data.push_back(mesh.Vertices[i + j].Position.X);
                    data.push_back(mesh.Vertices[i + j].Position.Y);
                    data.push_back(mesh.Vertices[i + j].Position.Z);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.X);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.Y);
                    t->setVertex(j, Vector4f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
                    t->setNormal(j, Vector3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
                    t->setTexCoord(j, Vector2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
                }
                TriangleList.push_back(t);
            }
        }
       
    }
    else {
        std::cout << "Model path invalid!" << std::endl;
    }
    //load texture
    QImage img;

    if (!img.load("C:/Users/dirk_/Documents/Games101/assignments/Hw3/Assignment3/Code/models/spot/spot_texture.png")) {

        qDebug() << "Image loading failed";
    }

    QImage t = (img.convertToFormat(QImage::Format_RGBA8888)).mirrored();
    texture = new QOpenGLTexture(t);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    vao.create();
    vao.bind();
    //create buffer (Do not release until VAO is created)
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(data.data(), sizeof(data[0])*data.size());
    // Create Vertex Array Object

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, sizeof(float) * 0, 3,sizeof(float)*5);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 2, sizeof(float) * 5);
    vao.release();
    vbo.release();
    m_program->release();
}
void GLwindow::paintGL() {

    f->glViewport(0, 0, 500, 500);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // issue some native OpenGL commands
    m_program->bind();
    QMatrix4x4 model;
    model.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 180));
    QMatrix4x4 view;
    QMatrix4x4 projection;
    projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    projection.translate(0, 0, -5);
    m_program->setUniformValue(modelUniform, model);
    m_program->setUniformValue(viewUniform, view);
    m_program->setUniformValue(projectionUniform, projection);
    //m_program->setUniformValue("text", 0);

    texture->bind();
    vao.bind();
    f->glDrawArrays(GL_TRIANGLES, 0, data.size());
    vao.release();
    texture->release();

    m_program->release();

    // animate continuously: schedule an update
    update();
}
void GLwindow::resizeGL(int w, int h) {
}