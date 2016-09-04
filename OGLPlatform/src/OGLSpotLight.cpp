#include "OGLSpotLight.hpp"
#include <GL/glew.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "OGLUtility.hpp"
#include "Model.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLCube.hpp"

OGLSpotLight::OGLSpotLight()
{
}

OGLSpotLight::~OGLSpotLight()
{
	glDisable(GL_DEPTH_TEST);
}

bool OGLSpotLight::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='Material Interactions' ";

	TwAddVarRW(tweakBar, "lightInner", TW_TYPE_FLOAT, &light.innerCutOff, "label='Spotlight radius' step='0.5' group='Light'");
	TwAddVarRW(tweakBar, "lightOut", TW_TYPE_FLOAT, &light.outerCutOff, "label='Spotlight radius' step='0.5' group='Light'");
	TwAddVarRW(tweakBar, "lightAmb", TW_TYPE_COLOR3F, &light.ambientColor, "label='Ambient Intensity' group='Light'");
	TwAddVarRW(tweakBar, "lightSpec", TW_TYPE_COLOR3F, &light.specularColor, "label='Specular Intensity' group='Light'");
	TwAddVarRW(tweakBar, "lightDiffus", TW_TYPE_COLOR3F, &light.diffuseColor, "label='Diffuse Intensity' group='Light'");

	TwDefine(format.str().c_str());

	return true;
}

bool OGLSpotLight::AddProgram(const std::string & vsFilename, const std::string & fsFilename)
{
	bool res = true;
	auto program = std::make_shared<gs::Program>();
	res &= program->CreateShader(GL_VERTEX_SHADER, vsFilename);
	res &= program->CreateShader(GL_FRAGMENT_SHADER, fsFilename);

	res &= program->Link();
	program->Use();
	programs.push_back(program);
	return res;
}

void OGLSpotLight::InitCubePosition()
{
	cubePositions[0] = glm::vec3(0);
	cubePositions[1] = glm::vec3(0, 20, 0);
	cubePositions[2] = glm::vec3(-10, 10, -12);
	cubePositions[3] = glm::vec3(-30, -7, -6);
	cubePositions[4] = glm::vec3(-28, -30, -10);
	cubePositions[5] = glm::vec3(-20, -12, -4);
	cubePositions[6] = glm::vec3(30, -12, -4);
	cubePositions[7] = glm::vec3(15, -10, -20);
	cubePositions[8] = glm::vec3(20, 0, -10);
	cubePositions[9] = glm::vec3(0, -20, -10);
}

void OGLSpotLight::InitLight()
{
	light.position = camera.GetPosition();
	light.direction = camera.GetForwardVector();
	light.ambientColor = glm::vec3(0.2f);
	light.diffuseColor = glm::vec3(0.8f);
	light.specularColor = glm::vec3(1.0f);
	light.innerCutOff = 12.5f;
	light.outerCutOff = 17.5f;
}

bool OGLSpotLight::AddLightUniform(gs::Program* program)
{
	bool res = true;
	res &= program->AddUniform("light.position");
	res &= program->AddUniform("light.direction");
	res &= program->AddUniform("light.ambientColor");
	res &= program->AddUniform("light.diffuseColor");
	res &= program->AddUniform("light.specularColor");
	res &= program->AddUniform("light.innerCutOff");
	res &= program->AddUniform("light.outerCutOff");
	return res;
}

bool OGLSpotLight::AddMatricesUniform(gs::Program * program)
{
	bool res = true;
	res &= program->AddUniform("MVP");
	res &= program->AddUniform("MV");
	res &= program->AddUniform("viewMatrix");
	res &= program->AddUniform("normalMatrix");
	return res;
}

bool OGLSpotLight::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}

	cubePositions.resize(10);

	res &= InitGUI();

	// Init Camera
	camera.SetPosition(glm::vec3(0, 0, 50));
	camera.SetSpeed(15.0f);
	camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);

	// Init light
	InitLight();	

	// Init materials
	InitCubePosition();

	// Init program
	res &= AddProgram("mesh.vert", "spotLight.frag");
	auto program = programs[0];

	// Get uniform locations
	res &= AddLightUniform(program.get());
	res &= AddMatricesUniform(program.get());
	res &= program->AddUniform("material.shininess");
	res &= program->AddUniform("samplerDiffuse1");
	program->AddUniform("samplerDiffuse2");
	program->AddUniform("samplerDiffuse3");
	program->AddUniform("samplerSpecular1");
	program->AddUniform("samplerSpecular2");

	glUniform1i(program->GetUniform("samplerDiffuse1"), 0);
	glUniform1i(program->GetUniform("samplerDiffuse2"), 1);
	glUniform1i(program->GetUniform("samplerDiffuse3"), 2);
	glUniform1i(program->GetUniform("samplerSpecular1"), 3);
	glUniform1i(program->GetUniform("samplerSpecular2"), 4);

	// Init geometry
	auto vao = std::make_shared<gs::VertexArray>();
	vaos.push_back(vao);
	vao->BindVAO();
	
	auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
	vbos.push_back(vbo);
	vbo->BindVBO();
	OGLCube cube;
	auto& vertices = cube.GetVertices();
	cube.InitVertices(glm::vec3(0));
	glBufferData(vbo->GetTarget(), sizeof(gs::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	vao->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), 0);
	vao->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
	vao->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));

	auto diffuse = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
	diffuse->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
	res &= diffuse->LoadTexture("containerDiffuse.dds");
	diffuse->BindTexture(GL_TEXTURE0);
	textures.push_back(diffuse);

	auto specular = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
	specular->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
	res &= specular->LoadTexture("containerSpecular.dds");
	specular->BindTexture(GL_TEXTURE3);
	textures.push_back(specular);	

	glEnable(GL_DEPTH_TEST);

	return res;
}

void OGLSpotLight::SendLightUniform(const gs::Program* program)
{
	glUniform3fv(program->GetUniform("light.position"), 1, glm::value_ptr(light.position));
	glm::vec4 lightDir = camera.GetViewMatrix() * glm::vec4(light.direction, 0.0f);
	glUniform3fv(program->GetUniform("light.direction"), 1, glm::value_ptr(glm::normalize(lightDir)));
	glUniform3fv(program->GetUniform("light.ambientColor"), 1, glm::value_ptr(light.ambientColor));
	glUniform3fv(program->GetUniform("light.diffuseColor"), 1, glm::value_ptr(light.diffuseColor));
	glUniform3fv(program->GetUniform("light.specularColor"), 1, glm::value_ptr(light.specularColor));
	glUniform1f(program->GetUniform("light.innerCutOff"), cosf(glm::radians(light.innerCutOff)));
	glUniform1f(program->GetUniform("light.outerCutOff"), cosf(glm::radians(light.outerCutOff)));
}

void OGLSpotLight::SendMatricesUniform(const gs::Program* program, glm::mat4 model)
{
	glUniformMatrix4fv(program->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
	glUniformMatrix4fv(program->GetUniform("MV"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix() * model));
	glUniformMatrix4fv(program->GetUniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(camera.GetViewMatrix() * model));
	glUniformMatrix4fv(program->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void OGLSpotLight::Render(double time)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::vec3 bgColor(0.2f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

	camera.Update();
	light.position = camera.GetPosition();
	light.direction = camera.GetForwardVector();

	// Render container
	auto programMesh = programs[0];
	programMesh->Use();

	SendLightUniform(programMesh.get());
		
	glm::mat4 model = glm::mat4(1);
	for (size_t i = 0; i < 10; i++) {
		float sign = (i % 2) * 2 - 1;
		model = glm::mat4(1);
		model = glm::rotate(model, i * 0.2f * glm::two_pi<float>(), glm::vec3(0, 1, 0)) *
				glm::translate(model, glm::vec3(20, 0, 20));

		model = glm::scale(glm::rotate(glm::translate(glm::mat4(1), cubePositions[i]), glm::radians(sign * 30.0f), cubePositions[i]), glm::vec3(10));
		SendMatricesUniform(programMesh.get(), model);
		vaos[0]->BindVAO();
		vbos[0]->BindVBO();
		textures[0]->BindTexture(GL_TEXTURE0);
		textures[1]->BindTexture(GL_TEXTURE3);
		glUniform1f(programMesh->GetUniform("material.shininess"), 32.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}