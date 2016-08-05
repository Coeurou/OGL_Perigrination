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
	glm::vec2 animation = glm::vec2(0.0f, 0.0f);

public:
	OGLAnimatedTriangle();
	~OGLAnimatedTriangle();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
    
private:
    bool InitGUI();
};