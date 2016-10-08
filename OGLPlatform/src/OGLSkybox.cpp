#include "OGLSkybox.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "OGLCube.hpp"
#include "OGLQuad.hpp"
#include <sstream>
#include <initializer_list>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


OGLSkybox::OGLSkybox()
{}

OGLSkybox::~OGLSkybox()
{
	glDisable(GL_DEPTH_TEST);
}

bool OGLSkybox::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Simple skybox Example' ";
    
	TwAddVarRW(tweakBar, "lightDir", TW_TYPE_DIR3F, &light.direction, "label='Direction' group='Light'");
	TwAddVarRW(tweakBar, "lightColor", TW_TYPE_COLOR3F, &light.diffuseColor, "label='Color' group='Light'");
	TwDefine(format.str().c_str());
    
    return true;
}

bool OGLSkybox::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();

		// Program MESHES
		auto programRender = std::make_shared<gs::Program>();
		res &= programRender->CreateShader(GL_VERTEX_SHADER, "mesh.vert");
		res &= programRender->CreateShader(GL_FRAGMENT_SHADER, "mesh.frag");

		res &= programRender->Link();
		programRender->Use();

		light.ambientColor = glm::vec3(0.2f);
		light.diffuseColor = glm::vec3(0.8f);
		light.specularColor = glm::vec3(1.0f);
		light.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		
		res &= programRender->AddUniform("MVP");
		programRender->AddUniform("MV");
		res &= programRender->AddUniform("normalMatrix");
		programRender->AddUniform("light.ambientColor");
		programRender->AddUniform("light.diffuseColor");
		programRender->AddUniform("light.specularColor");
		programRender->AddUniform("light.direction");
		programRender->AddUniform("material.shininess");
		programs.push_back(programRender);

		glUniform3fv(programs[0]->GetUniform("light.specularColor"), 1, glm::value_ptr(light.specularColor));
		glUniform3fv(programs[0]->GetUniform("light.ambientColor"), 1, glm::value_ptr(light.ambientColor));

		// Program GROUND
		auto programGround = std::make_shared<gs::Program>();
		res &= programGround->CreateShader(GL_VERTEX_SHADER, "simpleQuad.vert");
		res &= programGround->CreateShader(GL_FRAGMENT_SHADER, "simpleQuad.frag");

		res &= programGround->Link();
		programGround->Use();
		programGround->AddUniform("MVP");
		programs.push_back(programGround);
        
		// Texture CUBEMAP/SKYBOX
		skybox.Load("");

		// Texture GROUND
		auto textureGround = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
		res &= textureGround->LoadTexture("Sand.dds");
		textureGround->BindTexture(GL_TEXTURE0);
		textureGround->ChangeParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		textures.push_back(std::move(textureGround));
		
		// Data GROUND
		auto vaoGround = std::make_unique<gs::VertexArray>();
		vaoGround->BindVAO();
		auto vboGround = std::make_unique<gs::VertexBuffer>(GL_ARRAY_BUFFER);
		vboGround->BindVBO();
		vbos.push_back(std::move(vboGround));
		OGLQuad ground;
		ground.SetSize(1000);
		ground.InitVertices(glm::vec3(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * ground.GetVertices().size(), ground.GetVertices().data(), GL_STATIC_DRAW);
		vaoGround->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
		vaoGround->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
		vaos.push_back(std::move(vaoGround));

		// Data Spider
		renderedObjs.push_back(std::make_unique<gs::Model>());
		renderedObjs[0]->SetProgram(programRender);
		res &= renderedObjs[0]->Load("spider.obj");

		// Data Crysis Nanosuit
		renderedObjs.push_back(std::make_unique<gs::Model>());
		renderedObjs[1]->SetProgram(programRender);
		res &= renderedObjs[1]->Load("nanosuit.obj");

		// Camera Init
		camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
		camera.SetPosition(glm::vec3(0, 0, 15));
		camera.SetSpeed(15.0f);
	}
	glEnable(GL_DEPTH_TEST);

	return res;
}

void OGLSkybox::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Update();

	glm::mat4 MVP = camera.GetViewProjectionMatrix();  

	// Render GROUND
	programs[1]->Use();
	vaos[0]->BindVAO();
	textures[0]->BindTexture(GL_TEXTURE0);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f));
	model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(programs[1]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP * model));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Render SKYBOX
	skybox.SetMVP(MVP);
	skybox.SetCameraPosition(camera.GetPosition());
	skybox.Render();

	// Render Spider
	programs[0]->Use();
	glUniform3fv(programs[0]->GetUniform("light.diffuseColor"), 1, glm::value_ptr(light.diffuseColor));
	glUniform3fv(programs[0]->GetUniform("light.direction"), 1, glm::value_ptr(glm::normalize(light.direction)));

	model = glm::rotate(glm::mat4(1.0f), 45.0f * (float)glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));

	glUniformMatrix4fv(programs[0]->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP * model));
	renderedObjs[0]->Render();

	// Render Nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-20, -5, -5));
	model = glm::rotate(model, -45.0f * (float)glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f));
	MVP *= model;

	auto normalMatrix = glm::transpose(glm::inverse(camera.GetViewMatrix() * model));
	glUniformMatrix4fv(programs[0]->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(programs[0]->GetUniform("MV"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix() * model));
	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	renderedObjs[1]->Render();
}
