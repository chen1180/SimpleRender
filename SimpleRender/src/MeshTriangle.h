#pragma once
#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H
#include<qstring.h>
#include<vector>
#include<qopenglbuffer.h>`
#include<qopenglshaderprogram.h>
#include<qopenglvertexarrayobject.h>
#include"Object.h"
#include"Texture.h"
#include<qopenglfunctions.h>
#include<qopenglfunctions_4_3_core.h>
#include<qfileinfo.h>
#include<qdir.h>
#include<qmap.h>
struct Vertex {
	// position
	QVector3D Position;
	// normal
	QVector3D Normal;
	// texCoords
	QVector2D TexCoords;
};
struct  Mesh
{
	// Variable Set Constructor
	Mesh()
	{
		VAO = new QOpenGLVertexArrayObject();

		VBO = new QOpenGLBuffer();
	}
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	//Material material;
	QOpenGLVertexArrayObject* VAO;
	QOpenGLBuffer* VBO;
	//Texture* diffuseMap;
	//Texture* ambientMap;
	//Texture* specularMap;
	std::string matName;
};
class MeshTriangle :public Object
{
public:
	MeshTriangle(const QVector3D& pos) {
		translation = pos;

	}
	MeshTriangle() {
	}
	~MeshTriangle() {

	}
	void load(const QString& model_path);
	void setupBuffer(QOpenGLShaderProgram* m_program);
	void render(QOpenGLFunctions_4_3_Core* f);
	std::vector<Texture> textures;
	QMap<std::string, Texture*> diffuseMaps;
	QMap<std::string, Texture*> ambientMaps;
	QMap<std::string, Texture*> specularMaps;
	QMap<std::string, Material> materials;
	std::vector<Mesh> meshes;
};
#endif MESHTRIANGLE_H



