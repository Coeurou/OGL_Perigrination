#pragma once

#include "OGLStage.hpp"
#include <glm/glm.hpp>
#include <memory>

class OGLSimpleCube : public OGLStage
{
	bool isDynamicBg = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	std::shared_ptr<OGLObject> objRendered = nullptr;
	int ptSize = 0;

public:
	OGLSimpleCube();
	~OGLSimpleCube();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
};