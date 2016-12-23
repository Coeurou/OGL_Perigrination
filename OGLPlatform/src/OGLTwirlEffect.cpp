#include "OGLTwirlEffect.hpp"
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

OGLTwirlEffect::OGLTwirlEffect()
{
}

OGLTwirlEffect::~OGLTwirlEffect()
{
}

bool OGLTwirlEffect::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='Twirl effect Example' help='This example use the twirl mathematic formula to animate a texture.' ";

	TwDefine(format.str().c_str());

	TwAddVarRW(tweakBar, "Twirl", TW_TYPE_FLOAT, &twirlAmount, " label='Twirl amount' min='0.0' max='100.0' step='0.01' ");
	TwAddVarRW(tweakBar, "Radius", TW_TYPE_FLOAT, &maxRadius, " label='Radius' min='0.0' max='1.0' step='0.01' ");

	return true;
}

bool OGLTwirlEffect::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}
	res &= InitGUI();

	// Program Twirl effect
	auto program = std::make_shared<gs::Program>();
	res &= program->CreateShader(GL_VERTEX_SHADER, "twirlEffect.vert");
	res &= program->CreateShader(GL_FRAGMENT_SHADER, "twirlEffect.frag");

	res &= program->Link();
	program->Use();

	res &= program->AddUniform("twirlAmount");
	res &= program->AddUniform("maxRadius");

	programs.push_back(program);

	// Quad object
	quad.Load("");
	quad.SetProgram(program);

	// Texture
	auto texture = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
	texture->LoadTexture("engineflare1.DDS");
	texture->BindTexture(GL_TEXTURE0);
	texture->ChangeParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	texture->ChangeParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	textures.push_back(std::move(texture));

	// Camera init
	camera.SetSpeed(10);
	camera.SetPosition(glm::vec3(0, 0, 10));
	camera.SetupProjection(45.0f, windowWidth / (float)windowHeight, 0.01f);

	return res;
}

void OGLTwirlEffect::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT);

	camera.Update();

	glUniform1f(programs[0]->GetUniform("twirlAmount"), twirlAmount);
	glUniform1f(programs[0]->GetUniform("maxRadius"), maxRadius);

	quad.Render();
}