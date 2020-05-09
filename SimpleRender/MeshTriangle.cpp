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
    m_program->setAttributeBuffer(0, GL_FLOAT, sizeof(float) * 0, 3, sizeof(float) * 5);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 2, sizeof(float) * 5);
    vao.release();
    vbo.release();
    m_program->release();
}
void MeshTriangle::render(QOpenGLShaderProgram* m_program) {
    texture->bind();
    vao.bind();
    f->glDrawArrays(GL_TRIANGLES, 0, mesh.data.size());
    vao.release();
}