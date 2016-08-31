#pragma once

#include "Stage.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <array>

class OGLAnimatedPyramids : public gs::Stage
{
	bool isDynamicBg = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.5f, 1.0f);
	float speed = 2.0f;
	GLint mvLocation = 0;
	GLint projLocation = 0;
	std::array<glm::mat4, 5> modelView;
	glm::mat4 view;

public:
	OGLAnimatedPyramids();
	~OGLAnimatedPyramids();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;
    
private:
    bool InitGUI();
};