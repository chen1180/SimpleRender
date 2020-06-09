#pragma once
#include "Renderer.h"
class Rasteriser :
	public Renderer
{
public:
	Rasteriser(Scene* scene);
	virtual void init(QOpenGLFunctions_4_3_Core* f);
	virtual void finish();

	virtual void render(QOpenGLFunctions_4_3_Core* f);
	virtual void present() const;
	virtual void update(float secondsElapsed);
};

