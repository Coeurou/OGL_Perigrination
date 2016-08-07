#pragma once

#include "Stage.hpp"
#include "ATBGLEnumWrapper.h"
#include <vector>
#include <memory>

class OGLMipmaps : public gs::Stage
{
	TWTextureFilteringValue texMinFilterVal = TWTextureFilteringValue::TW_NEAREST;
	TWTextureFilteringValue texMagFilterVal = TWTextureFilteringValue::TW_NEAREST;

public:
	OGLMipmaps();
	~OGLMipmaps();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
    
private:
    bool InitGUI();
};