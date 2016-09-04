#pragma once

#include <GL/glew.h>
#include "Stage.hpp"

class OGLRipplePlane : public gs::Stage
{
    GLint timeLocation = 0;

public:
	OGLRipplePlane();
	~OGLRipplePlane();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;

private:
	bool InitGUI() override;
};

