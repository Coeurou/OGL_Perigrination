#pragma once

#include "OGLStage.hpp"
#include "ATBGLEnumWrapper.h"
#include <vector>
#include <memory>

class OGLArrayTexture : public OGLStage
{
public:
	OGLArrayTexture();
	~OGLArrayTexture();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
};