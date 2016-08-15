#pragma once

#include "Stage.hpp"
#include "ATBGLEnumWrapper.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class OGLModelMatrix : public gs::Stage
{
    GLint rX = 0.0f;
    GLint rY = 0.0f;
    GLint rZ = 0.0f;
    glm::vec3 tVector = glm::vec3(0.0f, 0.0f, -2.0f);

public:
	OGLModelMatrix();
	~OGLModelMatrix();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
    
private:
    bool InitGUI();
};