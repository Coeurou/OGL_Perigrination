#pragma once

#include <GL/glew.h>
#include "Stage.hpp"

class OGLRipplePlane : public gs::Stage
{
    GLint timeLocation = 0;

public:
	OGLRipplePlane();
	~OGLRipplePlane();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);

private:
	bool InitGUI();
};

