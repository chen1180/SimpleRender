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
	//Mesh(const Mesh&) = delete;
	//Mesh& operator=(const Mesh&) = delete;
	//Mesh(Mesh&&) = default;
	//Mesh& operator=(Mesh&&) = default;
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	Material material;
	QOpenGLVertexArrayObject* VAO;
	QOpenGLBuffer* VBO;
};
class MeshTriangle :public Object
{
public:
	MeshTriangle(const QVector3D& pos) {
		position = pos;
	}
	MeshTriangle() {
	}
	~MeshTriangle() {

	}
	void load(const QString& model_path);
	void setupBuffer(QOpenGLShaderProgram* m_program);
	void addTexture(const QString& path);
	void render(QOpenGLFunctions* f, QOpenGLShaderProgram* m_program);
	std::vector<float> data;
	std::vector<Mesh> meshes;
	Texture* texture;
};
#endif MESHTRIANGLE_H



