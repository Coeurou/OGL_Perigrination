#pragma once

#include "Stage.hpp"
#include <glm/glm.hpp>

class OGLPointSize : public gs::Stage
{
	bool isDynamicBg = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    
public:
	OGLPointSize();
	~OGLPointSize();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;

private:
    bool InitGUI();
};

