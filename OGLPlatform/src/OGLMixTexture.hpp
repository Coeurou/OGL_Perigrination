#pragma once

#include "OGLStage.hpp"
#include "ATBGLEnumWrapper.h"
#include <vector>
#include <memory>

class OGLMixTexture : public OGLStage
{
	GLint mixLocation = 0;
    GLint mvpLocation = 0;
    
public:
	OGLMixTexture();
	~OGLMixTexture();

	bool Init(int windowWidth, int windowHeight);
	void Render(double time);
    
private:
    bool InitGUI();
    bool InitShaders();
    bool InitTextures();
    bool InitGeometry();
    bool InitVBO();
};