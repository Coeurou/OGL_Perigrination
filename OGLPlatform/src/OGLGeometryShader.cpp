#include "OGLGeometryShader.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLQuad.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLGeometryShader::OGLGeometryShader()
{}

OGLGeometryShader::~OGLGeometryShader()
{
    glDisableVertexAttribArray(0);
}

bool OGLGeometryShader::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Geometry Shader Example' ";
    TwDefine(format.str().c_str());
    
    TwAddVarRW(tweakBar, "Div", TW_TYPE_INT32, &divisions, " label='Plane divisions' min='1' max ='8'");
    TwAddVarRW(tweakBar, "Distance", TW_TYPE_INT32, &distance, " label='Planes distance' ");
    
    return true;
}

bool OGLGeometryShader::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);

	if (res) {
        res &= InitGUI();
        auto vShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        vShader->SetSource("simplePosition.vert");
        vShader->Compile();
        auto program = std::make_shared<gs::Program>();
        program->Attach(vShader->get());
        
        auto gShader = std::make_shared<gs::Shader>(GL_GEOMETRY_SHADER);
        gShader->SetSource("doubleVertices.geom");
        gShader->Compile();
        program->Attach(gShader->get());
        
        auto fShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fShader->SetSource("geomColor.frag");
        fShader->Compile();
        program->Attach(fShader->get());
        program->Link();
        program->Use();
        programs.push_back(program);
        
        glm::mat4 T	= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -15.0f));
        glm::mat4 Rx = glm::rotate(T,  glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 MV = glm::rotate(Rx, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 MVP = projection * MV;
        mvpLocation = glGetUniformLocation(program->get(), "MVP");        
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
        
        divLocation = glGetUniformLocation(program->get(), "divisions");
        distLocation = glGetUniformLocation(program->get(), "dist");
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
        std::vector<glm::vec3> positions;
        positions.push_back(glm::vec3(-5.0f, 0.0f, -5.0f));
        positions.push_back(glm::vec3(5.0f, 0.0f, -5.0f));
        positions.push_back(glm::vec3(-5.0f, 0.0f, 5.0f));
        positions.push_back(glm::vec3(5.0f, 0.0f, 5.0f));

        vbo->BindVBO();
        glBufferData(vbo->GetTarget(), sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
        vbos.push_back(vbo);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	}
	return res;
}

void OGLGeometryShader::Render(double time)
{
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.2f, 1.0f)));
    glUniform1i(divLocation, divisions);
    glUniform1i(distLocation, distance);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
