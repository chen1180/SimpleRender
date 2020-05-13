#include "MeshTriangle.h"
#include"OBJ_Loader.h"


void MeshTriangle::load(const QString& model_path) {
    objl::Loader Loader;

    bool loadout = Loader.LoadFile(model_path.toStdString());

    if (loadout == true) {
        std::cout << "Model successfully loaded!" << std::endl;

        for (auto mesh : Loader.LoadedMeshes)
        {
            
            Mesh current_mesh;
            std::vector<Vertex> vertices;
            std::cout << "Mesh material name is:" << mesh.MeshMaterial.name << std::endl;
            for (int i = 0;i < mesh.Vertices.size();i += 3)
            {
                for (int j = 0;j < 3;j++)
                {
                    Vertex temp;
                    data.push_back(mesh.Vertices[i + j].Position.X);
                    data.push_back(mesh.Vertices[i + j].Position.Y);
                    data.push_back(mesh.Vertices[i + j].Position.Z);
                    data.push_back(mesh.Vertices[i + j].Normal.X);
                    data.push_back(mesh.Vertices[i + j].Normal.Y);
                    data.push_back(mesh.Vertices[i + j].Normal.Z);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.X);
                    data.push_back(mesh.Vertices[i + j].TextureCoordinate.Y);
                    temp.Position = QVector3D(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z);
                    temp.Normal = QVector3D(mesh.Vertices[i + j].Normal.X,mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z);
                    temp.TexCoords = QVector2D(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y);
                    vertices.push_back(temp);
                }

            }
            Material current_material;
            current_material.Ka = QVector3D(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z);
            current_material.Kd = QVector3D(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z);
            current_material.Ks = QVector3D(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z);
            current_material.Ns = mesh.MeshMaterial.Ns;
     
            //set mesh data
            current_mesh.vertices=vertices;
            current_mesh.material = current_material;
            meshes.push_back(current_mesh);
        }
          
           

    }
    else {
        std::cout << "Model path invalid!" << std::endl;
    }
}
void MeshTriangle::setupBuffer(QOpenGLShaderProgram* m_program) {
    /*m_program->bind();
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
    m_program->release();*/
    m_program->bind();
    for (auto& mesh : meshes) {

        mesh.VAO->create();
        mesh.VAO->bind();
        //create buffer (Do not release until VAO is created)
        mesh.VBO->create();
        mesh.VBO->bind();
        mesh.VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
        mesh.VBO->allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
        // Create Vertex Array Object

        m_program->enableAttributeArray(0);
        m_program->setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, Vertex::Position), 3, sizeof(Vertex));
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, Vertex::Normal), 3, sizeof(Vertex));
        m_program->enableAttributeArray(2);
        m_program->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, Vertex::TexCoords), 2, sizeof(Vertex));
        std::cout << "Noraml" << offsetof(Vertex, Vertex::Normal) << offsetof(Vertex, Vertex::TexCoords) << " " << sizeof(Vertex);
        mesh.VAO->release();
        mesh.VBO->release();
    }
    m_program->release();
}
void MeshTriangle::addTexture(const QString& path) {
    texture = new Texture(path);
    texture->bind();
}
void MeshTriangle::render(QOpenGLFunctions* f, QOpenGLShaderProgram* m_program) {
    for (auto& mesh : meshes) {
        m_program->setUniformValue("material.ka", mesh.material.Ka);
        m_program->setUniformValue("material.ks", mesh.material.Ks);
        m_program->setUniformValue("material.kd", mesh.material.Kd);
        m_program->setUniformValue("material.shiniess", mesh.material.Ns);
        mesh.VAO->bind();
        f->glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
        mesh.VAO->release();
    }
}