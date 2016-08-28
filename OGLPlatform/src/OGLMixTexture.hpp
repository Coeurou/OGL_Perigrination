#pragma once

#include "Stage.hpp"
#include "Camera.hpp"
#include "ATBGLEnumWrapper.h"
#include <vector>
#include <memory>


class OGLMixTexture : public gs::Stage
{
	const GLushort nbVertices = 100;
	const size_t halfWorldSize = 500;
	gs::Camera camera;

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