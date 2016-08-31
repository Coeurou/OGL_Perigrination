#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include <glm/glm.hpp>
#include <memory>

class OGLGeometryShader : public gs::Stage
{
    GLint divisions = 1;
    glm::mat4 viewMatrix;
    GLint distance = 1;
    GLint divLocation = 0;
    GLint mvpLocation = 0;
    GLint distLocation = 0;
    
public:
	OGLGeometryShader();
	~OGLGeometryShader();

	bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;

private:
    bool InitGUI();
};