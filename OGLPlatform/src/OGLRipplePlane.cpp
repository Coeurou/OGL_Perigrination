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
const int NUM_VERTICESX = 40;
const int NUM_VERTICESZ = 40;

OGLRipplePlane::OGLRipplePlane()
{
}

OGLRipplePlane::~OGLRipplePlane()
{
	glDisableVertexAttribArray(0);
}

void TW_CALL SwapWireframeMode(void* mode)
{
	auto newMode = static_cast<bool*>(mode);
	if (*newMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	*newMode = !(*newMode);
}

bool OGLRipplePlane::InitGUI()
{
	auto tweakBar = atbApp->GetBarByIndex(0);
	auto barName = TwGetBarName(tweakBar);
	std::stringstream format;
	format << barName << " " << " label='AnimatedTriangle Example' help='This example use glVertexAttrib* to animate a triangle.' ";

	TwDefine(format.str().c_str());
	TwAddButton(tweakBar, "Wireframe mode", SwapWireframeMode, reinterpret_cast<void*>(&polygonMode), " label='Enable wireframe' key=w");

	return true;
}

bool OGLRipplePlane::Init(int windowWidth, int windowHeight)
{
	bool res = gs::Stage::Init(windowWidth, windowHeight);
	if (!res) {
		return false;
	}
	res &= InitGUI();

	auto vShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
	vShader->SetSource("ripplePlane.vert");
	res &= vShader->Compile();
	auto program = std::make_shared<gs::Program>();
	program->Attach(vShader->get());

	auto fShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
	fShader->SetSource("simpleColor.frag");
	res &= fShader->Compile();
	program->Attach(fShader->get());

	program->Link();
	program->Use();
	programs.push_back(program);

	auto vao = std::make_shared<gs::VertexArray>();
	vao->BindVAO();
	vaos.push_back(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

	GLint mvpLocation = glGetUniformLocation(program->get(), "MVP");
	glm::mat4 MVP = projection * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) * glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	auto arrayVBO = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
	std::vector<glm::vec3> positions;
	for (size_t z = 0; z < NUM_VERTICESZ; z++) {
		for (size_t x = 0; x < NUM_VERTICESX; x++) {
			auto xCoord = ((float)x / ((NUM_VERTICESX - 1) * 2 - 1)) * HALF_SIZEX;
			auto zCoord = ((float)z / ((NUM_VERTICESZ - 1) * 2 - 1)) * HALF_SIZEZ;
			positions.push_back(glm::vec3(xCoord, 0.0f, zCoord));
		}
	}
	arrayVBO->BindVBO();
	glBufferData(arrayVBO->GetTarget(), sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	vbos.push_back(arrayVBO);

	auto indicesVBO = std::make_shared<gs::VertexBuffer>(GL_ELEMENT_ARRAY_BUFFER);
	indicesVBO->BindVBO();
	std::vector<int> indices;
	for (size_t z = 0; z < NUM_VERTICESZ; z++) {
		for (size_t x = 0; x < NUM_VERTICESX; x++) {
			int i0 = x + z * NUM_VERTICESX;
			int i1 = i0 + 1;
			int i2 = i0 + (z + 1) * NUM_VERTICESX;
			int i3 = i2 + 1;

			if ((x + z) % 2 == 0) {
				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i3);
			}
			else {
				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
		}
	}
	glBufferData(arrayVBO->GetTarget(), sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	vbos.push_back(indicesVBO);

	return res;
}

void OGLRipplePlane::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const int totalVertices = NUM_VERTICESX * NUM_VERTICESZ * 2 * 3;

	glDrawElements(GL_TRIANGLES, totalVertices, GL_UNSIGNED_SHORT, nullptr);
}