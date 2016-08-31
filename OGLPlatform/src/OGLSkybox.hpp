#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include <glm/glm.hpp>

class OGLSkybox : public gs::Stage
{
	gs::Camera camera;
	gs::DirectionnalLight light;

public:
	OGLSkybox();
	~OGLSkybox();

	bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;

private:
    bool InitGUI();
};