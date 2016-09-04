#include "OGLLight.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/type_ptr.hpp>

const std::string lightVSFilename = "oglLight.vert";
const std::string lightFSFilename = "oglLight.frag";

OGLLight::OGLLight() : vbo(GL_ARRAY_BUFFER), MVP(1.0f)
{}

OGLLight::~OGLLight() 
{}

bool OGLLight::Load(const std::string& meshFilename)
{
	bool res = true;

	vao.BindVAO();
	cube.InitVertices(glm::vec3(0));

	vbo.BindVBO();
	glBufferData(vbo.GetTarget(), sizeof(gs::Vertex) * cube.GetVertices().size(), cube.GetVertices().data(), GL_STATIC_DRAW);

	vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));

	res &= lightProgram.CreateShader(GL_VERTEX_SHADER, lightVSFilename);
	res &= lightProgram.CreateShader(GL_FRAGMENT_SHADER, lightFSFilename);
	lightProgram.Link();
	lightProgram.Use();

	lightProgram.AddUniform("MVP");

	return res;
}

void OGLLight::Render(gs::Program* program)
{
	lightProgram.Use();
	vao.BindVAO();
	glUniformMatrix4fv(lightProgram.GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}