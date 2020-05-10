#include "MeshTriangle.h"
#include"OBJ_Loader.h"

void MeshTriangle::load(const QString& model_path) {
    objl::Loader Loader;

    bool loadout = Loader.LoadFile(model_path.toStdString());

    if (loadout == true) {
        std::cout << "Model successfully loaded!" << std::endl;
        for (auto mesh : Loader.LoadedMeshes)
        {
            for (int i = 0;i < mesh.Vertices.size();i += 3)
            {
                for (int j = 0;j < 3;j++)
                {
                    data.push_back(mesh.Vertices[i + j].Position.X);
                    data.push_back(mesh.Vertices[i + j].Position.Y);
                    data.push_back(mesh.Vertices[i + j].Position.Z);
                    data.push_back(mesh.Vertices[i + j].Normal.X);
                    data.push_back(mesh.Vertices[i + j].Normal.Y);
                    data.push_back(mesh.Vertices[i + j].Normal.Z);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.X);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.Y);
                }
            }
        }

    }
    else {
        std::cout << "Model path invalid!" << std::endl;
    }
}
void MeshTriangle::setupBuffer(QOpenGLShaderProgram* m_program) {
    m_program->bind();
    vao.create();
    vao.bind();
    //create buffer (Do not release until VAO is created)
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(data.data(), sizeof(data[0]) *data.size());
    // Create Vertex Array Object

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, sizeof(float) * 0, 3, sizeof(float) * 8);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 3, sizeof(float) * 8);
    m_program->enableAttributeArray(2);
    m_program->setAttributeBuffer(2, GL_FLOAT, sizeof(float) * 6, 2, sizeof(float) * 8);
    vao.release();
    vbo.release();
    m_program->release();
}
void MeshTriangle::addTexture(const QString& path) {
    texture = new Texture(path);
    texture->bind();
}