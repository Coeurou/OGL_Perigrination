#include "OGLSkybox.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "OGLCube.hpp"
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
	TwAddVarRW(tweakBar, "lightColor", TW_TYPE_COLOR3F, &light.color, "label='Color' group='Light'");
	TwDefine(format.str().c_str());
    
    return true;
}

bool OGLSkybox::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        auto program = std::make_shared<gs::Program>();
		res &= program->CreateShader(GL_VERTEX_SHADER, "simpleSkybox.vert");
		res &= program->CreateShader(GL_FRAGMENT_SHADER, "simpleSkybox.frag");
        
		res &= program->Link();
        program->Use();
		program->AddUniform("MVP");
        programs.push_back(program);

		auto programRender = std::make_shared<gs::Program>();
		res &= programRender->CreateShader(GL_VERTEX_SHADER, "mesh.vert");
		res &= programRender->CreateShader(GL_FRAGMENT_SHADER, "mesh.frag");

		res &= programRender->Link();
		programRender->Use();

		light.color = glm::vec3(1.0f);
		light.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		light.ambientIntensity = 0.2f;
		light.diffuseIntensity = 0.8f;

		res &= programRender->AddUniform("MVP");
		res &= programRender->AddUniform("normalMatrix");
		programRender->AddUniform("light.color");
		programRender->AddUniform("light.direction");
		programRender->AddUniform("light.ambientIntensity");
		programRender->AddUniform("light.diffuseIntensity");
		programRender->AddUniform("material.diffuseColor");
		programRender->AddUniform("material.ambientColor");
		programRender->AddUniform("material.specularColor");
		programRender->AddUniform("material.emissiveColor");
		programRender->AddUniform("material.shininess");
		programs.push_back(programRender);
        
		auto texture = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
		res &= texture->LoadCubemap({ "posx.dds", "negx.dds", "posy.dds", "negy.dds", "posz.dds", "negz.dds" });
		texture->BindTexture(GL_TEXTURE0);
		textures.push_back(texture);

        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
		auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
		vbo->BindVBO();
		vbos.push_back(vbo);
		OGLCube cube;
		cube.InitVertices(glm::vec3(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * cube.GetVertices().size(), cube.GetVertices().data(), GL_STATIC_DRAW);
		vao->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));

		renderedObjs.push_back(std::make_unique<gs::Model>());
		res &= renderedObjs[0]->Load("spider.obj");

		camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
		camera.SetPosition(glm::vec3(0, 0, -15));
		camera.SetSpeed(15.0f);
	}
	glEnable(GL_DEPTH_TEST);

	return res;
}

void OGLSkybox::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int oldCullMode, oldDepthFunc;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullMode);
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    
    glCullFace(GL_FRONT_FACE);
    glDepthFunc(GL_LEQUAL);
	camera.Update();
	
	programs[0]->Use();
	vaos[0]->BindVAO();
	textures[0]->BindTexture(GL_TEXTURE0);
	glm::mat4 MVP = camera.GetViewProjectionMatrix();
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(glm::translate(MVP, camera.GetPosition())));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glCullFace(oldCullMode);
	glDepthFunc(oldDepthFunc);

	programs[1]->Use();
	glUniform3fv(programs[1]->GetUniform("light.color"), 1, glm::value_ptr(light.color));
	glUniform3fv(programs[1]->GetUniform("light.direction"), 1, glm::value_ptr(light.direction));
	glUniform1f(programs[1]->GetUniform("light.ambientIntensity"), light.ambientIntensity);
	glUniform1f(programs[1]->GetUniform("light.diffuseIntensity"), light.diffuseIntensity);

	glm::mat4 model = glm::rotate(glm::mat4(1.0f), 45.0f * (float)glm::radians(time), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));

	glUniformMatrix4fv(programs[1]->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(programs[1]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP * model));
	renderedObjs[0]->Render(programs[1].get());
    

}
