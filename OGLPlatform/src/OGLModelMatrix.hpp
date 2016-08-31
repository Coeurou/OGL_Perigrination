#pragma once

#include "Stage.hpp"
#include "ATBGLEnumWrapper.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class OGLModelMatrix : public gs::Stage
{
    GLint rX = 0;
    GLint rY = 0;
    GLint rZ = 0;
    glm::vec3 tVector = glm::vec3(0.0f, 0.0f, -2.0f);

public:
	OGLModelMatrix();
	~OGLModelMatrix();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;
    
private:
    bool InitGUI();
};