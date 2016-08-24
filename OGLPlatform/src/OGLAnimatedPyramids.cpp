#include "OGLAnimatedPyramids.hpp"
#include "OGLTriangle.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLUtility.hpp"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

OGLAnimatedPyramids::OGLAnimatedPyramids()
{}

OGLAnimatedPyramids::~OGLAnimatedPyramids()
{
	glDisable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool OGLAnimatedPyramids::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Animated pyramids Example' ";
    
    TwDefine(format.str().c_str());
    TwAddVarRW(tweakBar, "bgColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    TwAddVarRW(tweakBar, "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
	TwAddVarRW(tweakBar, "Speed", TW_TYPE_FLOAT, &speed, " label='Animation speed' min='1' max='10' step='0.5'");
    return true;
}

bool OGLAnimatedPyramids::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();

        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("pyramids.vert");
        res &= vertexShader->Compile();
        program->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("pyramids.frag");
        res &= fragmentShader->Compile();
        program->Attach(fragmentShader->get());
        
        res &= program->Link();
        program->Use();

		view = glm::lookAt(glm::vec3(0, 2, -10), glm::vec3(0), glm::vec3(0, 1, 0));
		mvLocation = glGetUniformLocation(program->get(), "ModelView");

		projLocation = glGetUniformLocation(program->get(), "Projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        
		auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
		vbo->BindVBO();

		std::vector<gs::ColoredVertex> vertices(5);
		vertices[0] = gs::ColoredVertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) };
		vertices[1] = gs::ColoredVertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) };
		vertices[2] = gs::ColoredVertex{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f) };
		vertices[3] = gs::ColoredVertex{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f) };
		vertices[4] = gs::ColoredVertex{ glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) };

		glBufferData(GL_ARRAY_BUFFER, sizeof(gs::ColoredVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		
		auto ibo = std::make_shared<gs::VertexBuffer>(GL_ELEMENT_ARRAY_BUFFER);
		ibo->BindVBO();

		std::vector<GLushort> indices(12);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 4;

		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 4;

		indices[6] = 1;
		indices[7] = 3;
		indices[8] = 4;

		indices[9] = 2;
		indices[10] = 3;
		indices[11] = 4;

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::ColoredVertex), (void*)offsetof(gs::ColoredVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gs::ColoredVertex), (void*)offsetof(gs::ColoredVertex, color));

        programs.push_back(program);
		vaos.push_back(vao);
		vbos.push_back(vbo);
		vbos.push_back(ibo);

		glEnable(GL_DEPTH_TEST);
    }
	return res;
}

void OGLAnimatedPyramids::Render(double time)
{
	float timeElapsed = (float)time;
	float angle = glm::radians(45.0f * timeElapsed) * speed;

	if (isDynamicBg) {
		bgColor.r = cosf(timeElapsed) * 0.5f + 0.5f;
		bgColor.g = sinf(timeElapsed) * 0.5f + 0.5f;
	}
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

	glm::mat4 currentModel(1.0f);
	currentModel = glm::rotate(view, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelView[0] = currentModel;

	currentModel = glm::translate(view, glm::vec3(-4.0f, 3.0f * sinf(angle), 0.0f));
	modelView[1] = currentModel;

	currentModel = glm::translate(view, glm::vec3(4.0f, 3.0f * sinf(angle), 0.0f));
	modelView[2] = currentModel;

	currentModel = glm::translate(view, glm::vec3(5.0f * sinf(angle), 4.0f, 0.0f));
	currentModel = glm::rotate(currentModel, angle, glm::vec3(1.0f, 0.0f, 0.0f));
	modelView[3] = currentModel;

	currentModel = glm::translate(view, glm::vec3(0.0f, -5.0f, 0.0f));
	currentModel = glm::rotate(currentModel, angle, glm::vec3(1.0f, 0.0f, 0.0f));
	currentModel = glm::rotate(currentModel, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	currentModel = glm::rotate(currentModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	float scaleFactor = sinf(timeElapsed) * 0.5f + 1.0f;
	currentModel = glm::scale(currentModel, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelView[4] = currentModel;

	glUniformMatrix4fv(mvLocation, 5, GL_FALSE, glm::value_ptr(*modelView.data()));
	glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, nullptr, 5);
}