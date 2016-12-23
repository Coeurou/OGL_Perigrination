#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Camera.hpp"
#include "OGLRenderableQuad.hpp"

class OGLTwirlEffect : public gs::Stage
{
	gs::Camera camera;
	OGLRenderableQuad quad;
	float twirlAmount = 0.0f;
	float maxRadius = 1.0f;

public:
	OGLTwirlEffect();
	~OGLTwirlEffect();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;

private:
	bool InitGUI() override;
};

