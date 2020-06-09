#pragma once
#include "Renderer.h"

class Scene;

class Raytracer :
	public Renderer
{
protected:
	unique_ptr<Quad> quad;
public:
	Raytracer(Scene* scene);
	virtual void init(QOpenGLFunctions_4_3_Core* f);
	virtual void finish();

	virtual void render(QOpenGLFunctions_4_3_Core* f);
	virtual void present() const;
	virtual void update(float secondsElapsed);
    inline float getSamples()const { return samplesCount; }

private:
	QOpenGLFramebufferObject *pathTraceFBO, *pathTraceFBOLowRes, *accumFBO;
	QOpenGLShaderProgram *pathTraceShader,  *pathTraceShaderLowRes,  *accumShader, *outputShader;
	GLuint pathTraceTexture, pathTraceTextureLowRes, accumTexture;
    QOpenGLBuffer* sceneBuffer;
    float samplesCount;
};

