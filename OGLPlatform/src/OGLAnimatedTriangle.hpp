#pragma once

#include "OGLStage.hpp"
#include <glm/glm.hpp>
#include <memory>

class OGLAnimatedTriangle : public OGLStage
{
	bool isDynamicBg = false;
	bool polygonMode = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.5f, 1.0f);
	glm::vec4 triangleColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 animation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	std::shared_ptr<OGLObject> objRendered = nullptr;
	int ptSize = 0;

public:
	OGLAnimatedTriangle();
	~OGLAnimatedTriangle();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
};