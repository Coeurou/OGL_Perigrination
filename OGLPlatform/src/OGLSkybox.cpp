#include "OGLSkybox.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "OGLCube.hpp"
#include <sstream>
#include <initializer_list>
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
    format << barName << " " << " label='SimpleTriangle Example' help='This example use GL_TRIANGLES.' ";
    
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

		camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
		camera.SetPosition(glm::vec3(0, 0, -5));
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
	glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
