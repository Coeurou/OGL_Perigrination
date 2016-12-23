#include "OGLMirror.hpp"
#include <GL/glew.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "OGLUtility.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"

OGLMirror::OGLMirror()
{
	glEnable(GL_DEPTH_TEST);
}

OGLMirror::~OGLMirror()
{
	glDisable(GL_DEPTH_TEST);
}

bool OGLMirror::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='Mirror effect Example' help='This example use framebuffer to add mirror effect.' ";

	TwDefine(format.str().c_str());

	return true;
}

bool OGLMirror::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}
	res &= InitGUI();

	// Program cube render
	auto program = std::make_shared<gs::Program>();
	res &= program->CreateShader(GL_VERTEX_SHADER, "simpleCube.vert");
	res &= program->CreateShader(GL_FRAGMENT_SHADER, "simpleCube.frag");

	res &= program->Link();
	program->Use();

	res &= program->AddUniform("MVP");

	programs.push_back(program);

	// Framebuffer binding
	framebuffer.BindFramebuffer(GL_DRAW_FRAMEBUFFER);
	renderbuffer.BindRenderbuffer(GL_RENDERBUFFER);
	glRenderbufferStorage(renderbuffer.GetRenderbufferTarget(), GL_DEPTH_COMPONENT, windowWidth, windowHeight);

	// Render to texture init
	glGenTextures(1, &renderTextureID);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

	// Render to texture binding
	glFramebufferTexture2D(framebuffer.GetFramebufferTarget(), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTextureID, 0);
	glFramebufferRenderbuffer(framebuffer.GetFramebufferTarget(), GL_DEPTH_ATTACHMENT,
							  renderbuffer.GetRenderbufferTarget(), renderbuffer.GetRenderbufferTarget());

	res &= framebuffer.CheckCompleteStatus();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(framebuffer.GetFramebufferTarget(), 0);

	// Cube object
	auto texture = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
	texture->LoadTexture("containerDiffuse.dds");
	texture->BindTexture(GL_TEXTURE0);
	textures.push_back(std::move(texture));

	cube.Load("");
	cube.SetProgram(program);

	// Mirror object
	mirror.SetSize(3.0f);
	mirror.Load("");
	mirror.SetProgram(program);

	// Camera init
	camera.SetSpeed(10);
	camera.SetPosition(glm::vec3(0, 0, 10));
	camera.SetupProjection(45.0f, windowWidth / (float)windowHeight, 0.01f);

	return res;
}

void OGLMirror::Render(double time)
{
	camera.Update();

	// Render to texture mirrored element in framebuffer
	framebuffer.BindFramebuffer(framebuffer.GetFramebufferTarget());	
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.976f)));
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 MVP = camera.GetViewProjectionMatrix();
	glm::vec3 V = glm::vec3(-MVP[2][0], -MVP[2][1], -MVP[2][2]);
	if (glm::dot(V, -mirrorNormal) < 0.0f) {
		glm::vec3 R = glm::reflect(V, mirrorNormal);
		MVP = glm::lookAt(glm::vec3(0), R, glm::vec3(0, 1, 0));
		MVP = glm::scale(MVP, glm::vec3(-1, 1, 1));
		glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

		cube.Render();
	}

	// Bind screen framebuffer and render cube + mirror
	glBindFramebuffer(framebuffer.GetFramebufferTarget(), 0);
	glDrawBuffer(GL_BACK_LEFT);

	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec3(1.0f, 0.945f, 0.316f)));
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
	glBindTexture(GL_TEXTURE_2D, renderTextureID);
	mirror.Render();

	textures[0]->BindTexture(GL_TEXTURE0);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 2));
	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
	cube.Render();
}