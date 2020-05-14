#include "MeshTriangle.h"
#include"OBJ_Loader.h"


void MeshTriangle::load(const QString& model_path) {
    objl::Loader Loader;

    bool loadout = Loader.LoadFile(model_path.toStdString());

    if (loadout == true) {
        std::cout << "Model successfully loaded!" << std::endl;
        //load mesh
        for (auto mesh : Loader.LoadedMeshes)
        {
            
            Mesh current_mesh;
            std::vector<Vertex> vertices;
            for (int i = 0;i < mesh.Vertices.size();i += 3)
            {
                for (int j = 0;j < 3;j++)
                {
                    Vertex temp;
                    temp.Position = QVector3D(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z);
                    temp.Normal = QVector3D(mesh.Vertices[i + j].Normal.X,mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z);
                    temp.TexCoords = QVector2D(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y);
                    vertices.push_back(temp);
                }

            }
            /*Material current_material;
            current_material.Ka = QVector3D(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z);
            current_material.Kd = QVector3D(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z);
            current_material.Ks = QVector3D(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z);
            current_material.Ns = mesh.MeshMaterial.Ns;
            current_material.name = mesh.MeshMaterial.name;
            QFileInfo full_path = QFileInfo(model_path);
            //// Get the name of the file without the extension
            QString dir = full_path.path();

            if (!mesh.MeshMaterial.map_Ka.empty())
            {
                current_material.map_Ka = QDir(dir).filePath(QString::fromStdString(mesh.MeshMaterial.map_Ka));
                current_material.have_map_Ka = true;
            }
        
            if (!mesh.MeshMaterial.map_Kd.empty()) {
                current_material.map_Kd = QDir(dir).filePath(QString::fromStdString(mesh.MeshMaterial.map_Kd));
                current_material.have_map_Kd = true;
            }
            //   
            if (!mesh.MeshMaterial.map_Ks.empty()) {
                current_material.map_Ks = QDir(dir).filePath(QString::fromStdString(mesh.MeshMaterial.map_Ks));
                current_material.have_map_Ks = true;
            }*/
            current_mesh.matName = mesh.MeshMaterial.name;
            //set mesh data
            current_mesh.vertices=vertices;
            //current_mesh.material = current_material;
            meshes.push_back(current_mesh);
        } 
        //load material
        for (auto material : Loader.LoadedMaterials)
        {
            Material current_material;
            current_material.Ka = QVector3D(material.Ka.X, material.Ka.Y, material.Ka.Z);
            current_material.Kd = QVector3D(material.Kd.X, material.Kd.Y, material.Kd.Z);
            current_material.Ks = QVector3D(material.Ks.X, material.Ks.Y, material.Ks.Z);
            current_material.Ns = material.Ns;
            current_material.name = material.name;
            QFileInfo full_path = QFileInfo(model_path);
            //// Get the name of the file without the extension
            QString dir = full_path.path();

            if (!material.map_Ka.empty())
            {
                current_material.map_Ka = QDir(dir).filePath(QString::fromStdString(material.map_Ka));
                Texture *texture =new Texture();
                if (texture->load(current_material.map_Ka)) {
                    ambientMaps.insert(current_material.name, texture);
                    current_material.have_map_Ka = true;
                }
            }

            if (!material.map_Kd.empty()) {
                current_material.map_Kd = QDir(dir).filePath(QString::fromStdString(material.map_Kd));
                Texture* texture = new Texture();
                if (texture->load(current_material.map_Kd)) {
                    diffuseMaps.insert(current_material.name, texture);
                    current_material.have_map_Kd = true;
                }
            }
            if (!material.map_Ks.empty()) {
                current_material.map_Ks = QDir(dir).filePath(QString::fromStdString(material.map_Ks));
                Texture* texture = new Texture();
                if (texture->load(current_material.map_Ks)) {
                    specularMaps.insert(current_material.name, texture);
                    current_material.have_map_Ks = true;
                }
            }
            materials.insert(current_material.name, current_material);
        }
    }
    else {
        std::cout << "Model path invalid!" << std::endl;
    }
}
void MeshTriangle::setupBuffer(QOpenGLShaderProgram* m_program) {
    m_program->bind();
    foreach(const auto & matName, materials.keys())
    {
        auto material = materials.value(matName);
        if (!material.map_Ka.isEmpty())
        {
            Texture* texture = new Texture();
            if (texture->load(material.map_Ka)) {
                ambientMaps.insert(material.name, texture);
                material.have_map_Ka = true;
                materials.insert(matName, material);
            }
        }

        if (!material.map_Kd.isEmpty())
        {
            Texture* texture = new Texture();
            if (texture->load(material.map_Kd)) {
                ambientMaps.insert(material.name, texture);
                material.have_map_Kd = true;
                materials.insert(matName, material);
            }
        }
        if (!material.map_Ka.isEmpty())
        {
            Texture* texture = new Texture();
            if (texture->load(material.map_Ka)) {
                ambientMaps.insert(material.name, texture);
                material.have_map_Ka = true;
                materials.insert(matName, material);
            }
        }
    }

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
        mesh.VAO->release();
        mesh.VBO->release();
    }
    m_program->release();
}

void MeshTriangle::render(QOpenGLFunctions* f, QOpenGLShaderProgram* m_program) {
    for (auto& mesh : meshes) {
        if (materials.contains(mesh.matName)) {
            Material tmp_material= materials.value(mesh.matName);
            //bind ambient texture
            if (tmp_material.have_map_Ka==true) {
                m_program->setUniformValue("materialTexture.ka", ambientMaps.value(mesh.matName)->id);
                ambientMaps.value(mesh.matName)->bind();
            }
            else {
                m_program->setUniformValue("material.ka", tmp_material.Ka);
            }
            //bind diffuse texture
            if (tmp_material.have_map_Kd == true) {
                m_program->setUniformValue("materialTexture.kd", diffuseMaps.value(mesh.matName)->id);
                diffuseMaps.value(mesh.matName)->bind();
            }
            else {
                m_program->setUniformValue("material.kd", tmp_material.Kd);
            }
            //bind specular texture
            if (tmp_material.have_map_Ks == true) {
                m_program->setUniformValue("materialTexture.ks", specularMaps.value(mesh.matName)->id);
                specularMaps.value(mesh.matName)->bind();
            }
            else {
                m_program->setUniformValue("material.ks", tmp_material.Ks);
            }
            m_program->setUniformValue("material.shiniess", tmp_material.Ns);
        }

        mesh.VAO->bind();
        f->glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
        mesh.VAO->release();
        if (materials.contains(mesh.matName)) {
            Material tmp_material = materials.value(mesh.matName);
            //bind ambient texture
            if (tmp_material.have_map_Ka == true) {
                ambientMaps.value(mesh.matName)->release();
            }
            //bind diffuse texture
            if (tmp_material.have_map_Kd == true) {
                diffuseMaps.value(mesh.matName)->release();
            }
            //bind specular texture
            if (tmp_material.have_map_Ks == true) {
                specularMaps.value(mesh.matName)->release();
            }
 
        }

  
    }
}