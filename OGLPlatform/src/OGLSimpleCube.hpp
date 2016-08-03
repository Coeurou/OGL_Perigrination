#pragma once

#include "OGLStage.hpp"
#include "ATBGLEnumWrapper.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class OGLSimpleCube : public OGLStage
{
	bool isDynamicBg = false;
	bool autoRotate = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::quat rotationAxis = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	std::shared_ptr<OGLObject> objRendered = nullptr;

public:
	OGLSimpleCube();
	~OGLSimpleCube();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
};