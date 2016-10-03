#include "Skybox.hpp"
#include "Program.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Skybox::Skybox() : texture(IMAGE_TYPE::GLI), vbo(GL_ARRAY_BUFFER), vao()
{
	program = std::make_shared<gs::Program>();
}

Skybox::~Skybox()
{ }

bool Skybox::Load(const std::string & cubeMapDirectory)
{
	bool res = true;

	res &= program->CreateShader(GL_VERTEX_SHADER, "simpleSkybox.vert");
	res &= program->CreateShader(GL_FRAGMENT_SHADER, "simpleSkybox.frag");
	res &= program->Link();
	program->Use();
	program->AddUniform("MVP");

	res &= texture.LoadCubemap({ cubeMapDirectory + "posx.dds", cubeMapDirectory + "negx.dds",
								 cubeMapDirectory + "posy.dds", cubeMapDirectory + "negy.dds",
								 cubeMapDirectory + "posz.dds", cubeMapDirectory + "negz.dds" });

	vao.BindVAO();
	vbo.BindVBO();
	OGLCube cube;
	cube.InitVertices(glm::vec3(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * cube.GetVertices().size(), cube.GetVertices().data(), GL_STATIC_DRAW);
	vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));

	return res;
}

void Skybox::Render()
{
	// Keep default depth comparison and cull mode
	int oldCullMode, oldDepthFunc;
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);

	// Render SKYBOX
	glCullFace(GL_FRONT_FACE);
	glDepthFunc(GL_LEQUAL);

	program->Use();
	vao.BindVAO();
	vbo.BindVBO();
	texture.BindTexture(GL_TEXTURE0);
	glUniformMatrix4fv(program->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(glm::translate(*mvpMatrix, *cameraPos)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glCullFace(oldCullMode);
	glDepthFunc(oldDepthFunc);	
}

void Skybox::Render(int nbInstances)
{
	std::cerr << "Skybox::Render(int nbInstances) not implemented." << std::endl;
}
