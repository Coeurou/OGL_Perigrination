#pragma once

#include "OGLStage.hpp"
#include <glm/glm.hpp>
#include <memory>

class OGLSimpleTriangle : public OGLStage
{
	bool isDynamicBg = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

public:
	OGLSimpleTriangle();
	~OGLSimpleTriangle();

	bool Init(int windowWidth, int windowHeight);
    void Render(double time);

private:
    bool InitGUI();
};