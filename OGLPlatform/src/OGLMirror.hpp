#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Renderbuffer.hpp"
#include "OGLRenderableQuad.hpp"
#include "OGLRenderableCube.hpp"

class OGLMirror : public gs::Stage
{
	gs::Framebuffer framebuffer;
	gs::Renderbuffer renderbuffer;
	gs::Camera camera;
	OGLRenderableQuad mirror;
	OGLRenderableCube cube;
	const glm::vec3 mirrorNormal = glm::vec3(0, 0, 1);

public:
	OGLMirror();
	~OGLMirror();

	bool Init(int windowWidth, int windowHeight) override;
	void Render(double time) override;

private:
	bool InitGUI() override;
	GLuint renderTextureID;
};

