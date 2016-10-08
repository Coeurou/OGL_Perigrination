#include "OGLMaterialInstancedDraw.hpp"
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

OGLMaterialInstancedDraw::OGLMaterialInstancedDraw()
{
}

OGLMaterialInstancedDraw::~OGLMaterialInstancedDraw()
{
	glDisable(GL_DEPTH_TEST);
}

bool OGLMaterialInstancedDraw::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='Material Interactions' ";

	TwAddVarRW(tweakBar, "lightDir", TW_TYPE_DIR3F, &light.direction, "label='Direction' group='Light'");
	TwAddVarRW(tweakBar, "lightAmb", TW_TYPE_COLOR3F, &light.ambientColor, "label='Ambient Intensity' group='Light'");
	TwAddVarRW(tweakBar, "lightSpec", TW_TYPE_COLOR3F, &light.specularColor, "label='Specular Intensity' group='Light'");
	TwAddVarRW(tweakBar, "lightDiffus", TW_TYPE_COLOR3F, &light.diffuseColor, "label='Diffuse Intensity' group='Light'");
	
	TwAddVarRW(tweakBar, "matAmb", TW_TYPE_COLOR3F, &material[0].ambientColor, "label='Ambient color' group='Material'");
	TwAddVarRW(tweakBar, "matDiffus", TW_TYPE_COLOR3F, &material[0].diffuseColor, "label='Diffuse color' group='Material'");
	TwAddVarRW(tweakBar, "matSpec", TW_TYPE_COLOR3F, &material[0].specularColor, "label='Specular color' group='Material'");
	TwAddVarRW(tweakBar, "matShine", TW_TYPE_FLOAT, &material[0].shininess, "label='Shininess' min='0' step='0.5' group='Material'");

	TwDefine(format.str().c_str());

	return true;
}

bool OGLMaterialInstancedDraw::AddProgram(const std::string & vsFilename, const std::string & fsFilename)
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

void OGLMaterialInstancedDraw::InitMaterials()
{
	material[0].ambientColor = glm::vec4(0.125f, 0.2225f, 0.1575f, 1.0f);
	material[0].diffuseColor = glm::vec4(0.54f, 0.89f, 0.63f, 1.0f);
	material[0].specularColor = glm::vec4(0.316228f, 0.316228f, 0.316228f, 1.0f);
	material[0].shininess = 76.8f;

	material[1].ambientColor = glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f);
	material[1].diffuseColor = glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	material[1].specularColor = glm::vec4(0.628281f, 0.555802f, 0.366065, 1.0f);
	material[1].shininess = 51.2f;

	material[2].ambientColor = glm::vec4(0.1745f, 0.01175f, 0.01175f, 1.0f);
	material[2].diffuseColor = glm::vec4(0.61424f, 0.04136f, 0.04136f, 1.0f);
	material[2].specularColor = glm::vec4(0.727811f, 0.626959f, 0.626959f, 1.0f);
	material[2].shininess = 76.8f;

	material[3].ambientColor = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	material[3].diffuseColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	material[3].specularColor = glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f);
	material[3].shininess = 76.8f;

	material[4].ambientColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material[4].diffuseColor = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f);
	material[4].specularColor = glm::vec4(0.50f, 0.50f, 0.50f, 0.25f);
	material[4].shininess = 32.0f;
}

void OGLMaterialInstancedDraw::InitLight()
{
	light.direction = glm::vec3(0.0f, 1.0f, 0.0f);
	light.ambientColor = glm::vec3(0.2f);
	light.diffuseColor = glm::vec3(0.8f);
	light.specularColor = glm::vec3(1.0f);
}

bool OGLMaterialInstancedDraw::AddLightUniform(gs::Program* program)
{
	bool res = true;
	res &= program->AddUniform("light.direction");
	res &= program->AddUniform("light.ambientColor");
	res &= program->AddUniform("light.diffuseColor");
	res &= program->AddUniform("light.specularColor");
	return res;
}

bool OGLMaterialInstancedDraw::AddMatricesUniform(gs::Program * program)
{
	bool res = true;
	res &= program->AddUniform("MVP");
	res &= program->AddUniform("MV");
	res &= program->AddUniform("normalMatrix");
	return res;
}

bool OGLMaterialInstancedDraw::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}
	material.resize(5);

	res &= InitGUI();

	// Init program
	res &= AddProgram("material.vert", "material.frag");
	auto program = programs[0];

	// Init light
	InitLight();	

	// Init materials
	InitMaterials();

	// Get uniform locations
	res &= AddLightUniform(program.get());
	res &= program->AddUniform("material.ambientColor");
	res &= program->AddUniform("material.diffuseColor");
	res &= program->AddUniform("material.specularColor");
	res &= program->AddUniform("material.shininess");
	res &= AddMatricesUniform(program.get());
    
	// Init program for specular mesh rendering
	res &= AddProgram("mesh.vert", "mesh.frag");
	auto programMesh = programs[1];

	// Get uniform locations
	res &= AddLightUniform(programMesh.get());
	res &= programMesh->AddUniform("material.shininess");
	res &= AddMatricesUniform(programMesh.get());
	res &= programMesh->AddUniform("samplerDiffuse1");
	programMesh->AddUniform("samplerDiffuse2");
	programMesh->AddUniform("samplerDiffuse3");
	programMesh->AddUniform("samplerSpecular1");
	programMesh->AddUniform("samplerSpecular2");

	// Init models
	renderedObjs.push_back(std::make_unique<gs::Model>());
	renderedObjs[0]->SetProgram(program);
	res &= renderedObjs[0]->Load("sphere.obj");

	renderedObjs.push_back(std::make_unique<gs::Model>());
	renderedObjs[1]->SetProgram(programMesh);
	res &= renderedObjs[1]->Load("palette.obj");

	auto vao = std::make_unique<gs::VertexArray>();
	vao->BindVAO();
	
	auto vbo = std::make_unique<gs::VertexBuffer>(GL_ARRAY_BUFFER);
	vbo->BindVBO();
	OGLCube cube;
	auto& vertices = cube.GetVertices();
	cube.InitVertices(glm::vec3(0));
	glBufferData(vbo->GetTarget(), sizeof(gs::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	vbos.push_back(std::move(vbo));

	vao->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), 0);
	vao->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
	vao->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));
	vaos.push_back(std::move(vao));

	auto diffuse = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
	diffuse->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
	res &= diffuse->LoadTexture("containerDiffuse.dds");
	diffuse->BindTexture(GL_TEXTURE0);
	textures.push_back(std::move(diffuse));

	auto specular = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
	specular->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
	res &= specular->LoadTexture("containerSpecular.dds");
	specular->BindTexture(GL_TEXTURE3);
	textures.push_back(std::move(specular));

	// Init Camera
	camera.SetPosition(glm::vec3(0, 0, 30));
	camera.SetSpeed(15.0f);
	camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);

	glEnable(GL_DEPTH_TEST);

	return res;
}

void OGLMaterialInstancedDraw::SendLightUniform(const gs::Program* program)
{
	glUniform3fv(program->GetUniform("light.direction"), 1, glm::value_ptr(glm::normalize(light.direction)));
	glUniform3fv(program->GetUniform("light.ambientColor"), 1, glm::value_ptr(light.ambientColor));
	glUniform3fv(program->GetUniform("light.diffuseColor"), 1, glm::value_ptr(light.diffuseColor));
	glUniform3fv(program->GetUniform("light.specularColor"), 1, glm::value_ptr(light.specularColor));
}

void OGLMaterialInstancedDraw::SendMatricesUniform(const gs::Program* program, glm::mat4 model)
{
	glUniformMatrix4fv(program->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
	glUniformMatrix4fv(program->GetUniform("MV"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix() * model));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(camera.GetViewMatrix() * model));
	glUniformMatrix4fv(program->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void OGLMaterialInstancedDraw::Render(double time)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &light.diffuseColor[0]);

	camera.Update();

	// Render spheres
	auto program = programs[0];
	program->Use();

	SendLightUniform(program.get());
		
	glm::mat4 model = glm::mat4(1);
	for (size_t i = 0; i < 5; i++) {
		model = glm::mat4(1);
		model = glm::rotate(model, i * 0.2f * glm::two_pi<float>(), glm::vec3(0, 1, 0)) *
				glm::translate(model, glm::vec3(20, 0, 20));

		SendMatricesUniform(program.get(), model);

		glUniform3fv(program->GetUniform("material.ambientColor"), 1, glm::value_ptr(material[i].ambientColor));
		glUniform3fv(program->GetUniform("material.diffuseColor"), 1, glm::value_ptr(material[i].diffuseColor));
		glUniform3fv(program->GetUniform("material.specularColor"), 1, glm::value_ptr(material[i].specularColor));
		glUniform1f(program->GetUniform("material.shininess"), material[i].shininess);

		renderedObjs[0]->Render();
	}

	auto programMesh = programs[1];
	programMesh->Use();
	
	// Render palette
	model = glm::scale(glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, -20, 0)), glm::radians(30.0f * (float)time), glm::vec3(0,1,0)), glm::vec3(3));
	SendLightUniform(programMesh.get());
	SendMatricesUniform(programMesh.get(), model);
	renderedObjs[1]->Render();

	// Render container
	model = glm::scale(glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, -10, 0)), glm::radians(-30.0f * (float)time), glm::vec3(0, 1, 0)), glm::vec3(10));
	SendMatricesUniform(programMesh.get(), model);
	vaos[0]->BindVAO();
	vbos[0]->BindVBO();
	textures[0]->BindTexture(GL_TEXTURE0);
	textures[1]->BindTexture(GL_TEXTURE3);
	glUniform1f(programMesh->GetUniform("material.shininess"), 32.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}