#pragma once
#include"Scene.h"
#include"global.h"
#include"quad.h"
#include<qopenglbuffer.h>`
#include<qopenglvertexarrayobject.h>
class Scene;
class Renderer :protected QOpenGLFunctions_4_3_Core{
protected:
	Scene* scene;
	GLuint tex, accum_tex, progressive_tex;
	unique_ptr<Quad> quad;
	bool initialized;
	int numOfLights;
public:
	Renderer(Scene* scene):scene(scene) {
	};
	virtual void init()=0;
	virtual void finish()=0;

	virtual void render(QOpenGLFunctions_4_3_Core* f) = 0;
	virtual void present() const = 0;
	virtual void update(float secondsElapsed)=0;
private:
	GLuint pathTraceFBO, pathTraceFBOLowRes, accumFBO, outputFBO;
	unique_ptr<QOpenGLShaderProgram> pathTraceShader, pathTraceShaderLowRes, accumShader, outputShader;
	GLuint pathTraceTexture, pathTraceTextureLowRes, accumTexture, tileOutputTexture[2];


};