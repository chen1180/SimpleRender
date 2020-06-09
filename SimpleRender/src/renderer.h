#pragma once
#include"Scene.h"
#include"Global.h"
#include"Quad.h"

class Scene;
class Renderer{
protected:
	Scene* scene;
	bool initialized=false;
	int numOfLights;
public:
	Renderer(Scene* scene):scene(scene) {
	};
	virtual void init(QOpenGLFunctions_4_3_Core* f)=0;
	virtual void finish()=0;

	virtual void render(QOpenGLFunctions_4_3_Core* f) = 0;
	virtual void present() const = 0;
	virtual void update(float secondsElapsed)=0;


};