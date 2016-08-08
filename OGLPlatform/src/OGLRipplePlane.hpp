#pragma once

#include "Stage.hpp"

class OGLRipplePlane : public gs::Stage
{
	bool polygonMode;

public:
	OGLRipplePlane();
	~OGLRipplePlane();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);

private:
	bool InitGUI();
};

