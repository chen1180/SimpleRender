#include "Scene.h"
void Scene::Render(QOpenGLFunctions* f) {
    // issue some native OpenGL commands
    QMatrix4x4 view = arcball.transform();

    QMatrix4x4 projection;
    projection.perspective(fov, (float)width / height, 0.1f, 1000.0f);


    for (auto& b : get_objects()) {
        MeshTriangle& x = dynamic_cast<MeshTriangle&>(*b);
        int i = 0;

        x.shader_program->bind();
        x.shader_program->setUniformValue("view", view);
        x.shader_program->setUniformValue("projection", projection);
        x.shader_program->setUniformValue("viewPos", arcball.eye());
        for (auto& light : get_lights())
        {
            Light& l = *light;
            x.shader_program->setUniformValue(QString("light[%1].position").arg(i).toStdString().c_str(), l.position);
            x.shader_program->setUniformValue(QString("light[%1].ambient").arg(i).toStdString().c_str(), l.ambient);
            x.shader_program->setUniformValue(QString("light[%1].diffuse").arg(i).toStdString().c_str(), l.diffuse);
            x.shader_program->setUniformValue(QString("light[%1].specular").arg(i).toStdString().c_str(), l.specular);
            i++;
        }


        QMatrix4x4 model;
        model.scale(x.scale);
        //model.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 180));
        model.translate(x.translation);
        x.shader_program->setUniformValue("model", model);

        //x.texture->bind();
        /*x.vao.bind();
        f->glDrawArrays(GL_TRIANGLES, 0, x.meshes.vertices.size());*/
        //x.texture->release();
        x.render(f);
        x.shader_program->release();
    }


    for (auto& light : get_lights())
    {
        Light& l = *light;
        l.shape->shader_program->bind();
        l.shape->shader_program->setUniformValue("view", view);
        l.shape->shader_program->setUniformValue("projection", projection);
        QMatrix4x4 model;
        model.translate(l.position);
        model.scale(QVector3D(0.1, 0.1, 0.1));

        l.shape->shader_program->setUniformValue("model", model);
        l.shape->render(f);
        l.shape->shader_program->release();

    }

}