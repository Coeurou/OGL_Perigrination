#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>

class OGLSkybox : public gs::Stage
{
	gs::Camera camera;

public:
	OGLSkybox();
	~OGLSkybox();

	bool Init(int windowWidth, int windowHeight);
    void Render(double time);

private:
    bool InitGUI();
};