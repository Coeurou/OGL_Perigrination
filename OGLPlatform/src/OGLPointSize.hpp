#pragma once

#include "OGLStage.hpp"
#include <glm/glm.hpp>

class OGLPointSize : public OGLStage
{
	bool isDynamicBg = false;
	glm::vec4 bgColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    
public:
	OGLPointSize();
	~OGLPointSize();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);

private:
    bool InitGUI();
};

