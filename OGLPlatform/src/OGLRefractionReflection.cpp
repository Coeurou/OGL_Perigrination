#include "OGLRefractionReflection.hpp"
#include "Program.hpp"
#include "Model.hpp"
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


OGLRefractionReflection::OGLRefractionReflection()
{}

OGLRefractionReflection::~OGLRefractionReflection()
{
	glDisable(GL_DEPTH_TEST);
}

bool OGLRefractionReflection::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Reflection/Refraction Example' ";
    
	// Material GUI
	TwAddVarRW(tweakBar, "ambientColor", TW_TYPE_COLOR4F, glm::value_ptr(material.ambientColor), " label='Ambient color' group='Material' ");
	TwAddVarRW(tweakBar, "diffuseColor", TW_TYPE_COLOR4F, glm::value_ptr(material.diffuseColor), " label='Diffuse color' group='Material' ");
	TwAddVarRW(tweakBar, "specularColor", TW_TYPE_COLOR4F, glm::value_ptr(material.specularColor), " label='Specular color' group='Material' ");
	TwAddVarRW(tweakBar, "shininess", TW_TYPE_FLOAT, &material.shininess, " label='Material shininess' min='0.0' step='0.5' group='Material' ");

	// Light GUI
	TwAddVarRW(tweakBar, "lightAmbColor", TW_TYPE_COLOR3F, glm::value_ptr(sunLight.ambientColor), " label='Light Ambient Color' group='Light' ");
	TwAddVarRW(tweakBar, "lightDiffColor", TW_TYPE_COLOR3F, glm::value_ptr(sunLight.diffuseColor), " label='Light Diffuse Color' group='Light' ");
	TwAddVarRW(tweakBar, "lightSpecColor", TW_TYPE_COLOR3F, glm::value_ptr(sunLight.specularColor), " label='Light Specular Color' group='Light' ");
	TwAddVarRW(tweakBar, "lightDir", TW_TYPE_DIR3F, glm::value_ptr(sunLight.direction), " label='Light Direction' group='Light' ");

	// Reflection/Refraction GUI
	TwAddVarRW(tweakBar, "ReflectIntensity", TW_TYPE_FLOAT, &reflectIntensity, " label='Reflection Intensity' min='0.0' max='1.0' step='0.01' group='Reflection/Refraction' ");
	TwAddVarRW(tweakBar, "RefractIntensity", TW_TYPE_FLOAT, &refractIntensity, " label='Refraction Intensity' min='0.0' step='0.01' group='Reflection/Refraction' ");
	TwAddVarRW(tweakBar, "RefractIndex1", TW_TYPE_FLOAT, &refractIndex1, " label='Refractive Index 1' min='0.0' step='0.01' group='Reflection/Refraction' ");
	TwAddVarRW(tweakBar, "RefractIndex2", TW_TYPE_FLOAT, &refractIndex2, " label='Refractive Index 2' min='0.0' step='0.01' group='Reflection/Refraction' ");
	TwDefine(format.str().c_str());
    
    return true;
}

void OGLRefractionReflection::InitMaterial()
{
	material.ambientColor = glm::vec4(0.0f, 0.05f, 0.0f, 1.0f);
	material.diffuseColor = glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);
	material.specularColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	material.shininess = 32.0f;
}

void OGLRefractionReflection::InitSunLight()
{
	sunLight.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	sunLight.diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	sunLight.specularColor = glm::vec3(1.0f);
	sunLight.direction = glm::vec3(0, 0, -1);
}

bool OGLRefractionReflection::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
		InitMaterial();
		InitSunLight();
		
        res &= InitGUI();

		// Program Reflection/Refraction
		auto programRender = std::make_shared<gs::Program>();
		res &= programRender->CreateShader(GL_VERTEX_SHADER, "refractionReflection.vert");
		res &= programRender->CreateShader(GL_FRAGMENT_SHADER, "refractionReflection.frag");

		res &= programRender->Link();
		programRender->Use();
		
		// Vertex shader uniforms
		res &= programRender->AddUniform("model");
		res &= programRender->AddUniform("view");
		res &= programRender->AddUniform("projection");
		res &= programRender->AddUniform("normalMatrix");
		res &= programRender->AddUniform("eyePosition");

		// Fragment shader uniforms
		res &= programRender->AddUniform("reflectionIntensity");
		res &= programRender->AddUniform("refractionIntensity");
		res &= programRender->AddUniform("refractIndex1");
		res &= programRender->AddUniform("refractIndex2");

		res &= programRender->AddUniform("material.ambientColor");
		res &= programRender->AddUniform("material.diffuseColor");
		res &= programRender->AddUniform("material.specularColor");
		res &= programRender->AddUniform("material.shininess");

		res &= programRender->AddUniform("light.ambientColor");
		res &= programRender->AddUniform("light.diffuseColor");
		res &= programRender->AddUniform("light.specularColor");
		res &= programRender->AddUniform("light.direction");
		res = true; // DEBUG hack
		programs.push_back(programRender);
        
		// Skybox
		skybox.Load("");		

		// Sphere model
		renderedObjs.push_back(std::make_unique<gs::Model>());
		renderedObjs[0]->SetProgram(programRender);
		res &= renderedObjs[0]->Load("sphere.obj");

		// Camera Init
		camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
		camera.SetPosition(glm::vec3(0, 0, 15));
		camera.SetSpeed(15.0f);
	}
	glEnable(GL_DEPTH_TEST);

	return res;
}

void OGLRefractionReflection::SendVSUniforms(const glm::mat4& modelMatrix)
{
	glUniformMatrix4fv(programs[0]->GetUniform("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(programs[0]->GetUniform("view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(programs[0]->GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	glUniformMatrix4fv(programs[0]->GetUniform("normalMatrix"), 1, GL_TRUE, glm::value_ptr(glm::inverse(modelMatrix)));
	glUniform3fv(programs[0]->GetUniform("eyePosition"), 1, glm::value_ptr(camera.GetPosition()));
}

void OGLRefractionReflection::SendMaterialUniforms()
{
	glUniform4fv(programs[0]->GetUniform("material.ambientColor"), 1, glm::value_ptr(material.ambientColor));
	glUniform4fv(programs[0]->GetUniform("material.diffuseColor"), 1, glm::value_ptr(material.diffuseColor));
	glUniform4fv(programs[0]->GetUniform("material.specularColor"), 1, glm::value_ptr(material.specularColor));
	glUniform1f(programs[0]->GetUniform("material.shininess"), material.shininess);
}

void OGLRefractionReflection::SendSunLightUniforms()
{
	glUniform3fv(programs[0]->GetUniform("light.ambientColor"), 1, glm::value_ptr(sunLight.ambientColor));
	glUniform3fv(programs[0]->GetUniform("light.diffuseColor"), 1, glm::value_ptr(sunLight.diffuseColor));
	glUniform3fv(programs[0]->GetUniform("light.specularColor"), 1, glm::value_ptr(sunLight.specularColor));
	glUniform3fv(programs[0]->GetUniform("light.direction"), 1, glm::value_ptr(glm::normalize(sunLight.direction)));
}

void OGLRefractionReflection::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Update();
	
	glCullFace(GL_BACK);

	// Render SKYBOX
	skybox.SetMVP(camera.GetViewProjectionMatrix());
	skybox.SetCameraPosition(camera.GetPosition());
	skybox.Render();

	// Render Sphere
	programs[0]->Use();	
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), 45.0f * glm::radians((float)time), glm::vec3(0.0f, 1.0f, 0.0f));

	SendVSUniforms(model);
	SendMaterialUniforms();
	SendSunLightUniforms();

	glUniform1f(programs[0]->GetUniform("reflectionIntensity"), reflectIntensity);
	glUniform1f(programs[0]->GetUniform("refractionIntensity"), refractIntensity);
	glUniform1f(programs[0]->GetUniform("refractIndex1"), refractIndex1);
	glUniform1f(programs[0]->GetUniform("refractIndex2"), refractIndex2);

	renderedObjs[0]->Render();
}
