#include "OGLRipplePLane.hpp"
#include <GL/glew.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "OGLUtility.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"

const int HALF_SIZEX = 2;
const int HALF_SIZEZ = 2;
const int NUM_VERTICESX = 100;
const int NUM_VERTICESZ = 100;
int totalVertices = 0;

OGLRipplePlane::OGLRipplePlane()
{
}

OGLRipplePlane::~OGLRipplePlane()
{
	glDisableVertexAttribArray(0);
}

bool OGLRipplePlane::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='Ripple plane Example' help='This example use the wave mathematic formula to animate a plane.' ";

	TwDefine(format.str().c_str());

	return true;
}

bool OGLRipplePlane::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}
	res &= InitGUI();

	auto vShader = std::make_unique<gs::Shader>(GL_VERTEX_SHADER);
	vShader->SetSource("ripplePlane.vert");
	res &= vShader->Compile();
	auto program = std::make_shared<gs::Program>();
	program->Attach(vShader->get());

	auto fShader = std::make_unique<gs::Shader>(GL_FRAGMENT_SHADER);
	fShader->SetSource("simpleColor.frag");
	res &= fShader->Compile();
	program->Attach(fShader->get());

	res &= program->Link();
	program->Use();
	programs.push_back(program);

    GLint mvpLocation = glGetUniformLocation(program->get(), "MVP");
    timeLocation = glGetUniformLocation(program->get(), "time");
    glm::mat4 T	= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 Rx = glm::rotate(T,  glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV = glm::rotate(Rx, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP = projection * MV;
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
    
	auto vao = std::make_unique<gs::VertexArray>();
	vao->BindVAO();
	vaos.push_back(std::move(vao));

	auto vbo = std::make_unique<gs::VertexBuffer>(GL_ARRAY_BUFFER);
	std::vector<glm::vec3> positions;
	for (size_t z = 0; z <= NUM_VERTICESZ; z++) {
		for (size_t x = 0; x <= NUM_VERTICESX; x++) {
			auto xCoord = ((((float)x / ((NUM_VERTICESX - 1)) * 2) - 1)) * HALF_SIZEX;
			auto zCoord = ((((float)z / ((NUM_VERTICESZ - 1)) * 2) - 1)) * HALF_SIZEZ;
			positions.push_back(glm::vec3(xCoord, 0.0f, zCoord));
		}
	}
	vbo->BindVBO();
	glBufferData(vbo->GetTarget(), sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	vbos.push_back(std::move(vbo));
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	auto ibo = std::make_unique<gs::VertexBuffer>(GL_ELEMENT_ARRAY_BUFFER);
	ibo->BindVBO();
	std::vector<GLushort> indices;
	for (GLushort z = 0; z < NUM_VERTICESZ; z++) {
		for (GLushort x = 0; x < NUM_VERTICESX; x++) {
			GLushort i0 = x + (z * (NUM_VERTICESX+1));
			GLushort i1 = i0 + 1;
			GLushort i2 = i0 + (NUM_VERTICESX+1);
			GLushort i3 = i2 + 1;

			if ((x + z) % 2 == 1) {
				indices.push_back(i0);
				indices.push_back(i2);
				indices.push_back(i1);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
            else {
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
				indices.push_back(i0);
				indices.push_back(i3);
				indices.push_back(i1);
			}
		}
	}
	glBufferData(ibo->GetTarget(), sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    totalVertices = (int)indices.size();
	vbos.push_back(std::move(ibo));

	return res;
}

void OGLRipplePlane::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1f(timeLocation, (float)time * 2.0f);
	glDrawElements(GL_TRIANGLES, totalVertices, GL_UNSIGNED_SHORT, (void*)0);
}