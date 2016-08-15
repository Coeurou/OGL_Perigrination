//
//  OGLInstancedDraw.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLInstancedDraw.hpp"
#include "OGLInstancedDraw.hpp"
#include "OGLPoint.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

OGLInstancedDraw::OGLInstancedDraw()
{}

OGLInstancedDraw::~OGLInstancedDraw()
{}

bool OGLInstancedDraw::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " <<  " label='Draw instanced Example' help='This example use instance IDs & billboarding.' ";
    
    return true;
}

bool OGLInstancedDraw::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();
        
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        auto geometryShader = std::make_shared<gs::Shader>(GL_GEOMETRY_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("instancedPoint.vert");
        res &= vertexShader->Compile();
        fragmentShader->SetSource("animatedTriangle.frag");
        res &= fragmentShader->Compile();
        geometryShader->SetSource("PointToQuad.geom");
        res &= geometryShader->Compile();
        
        program->Attach(vertexShader->get());
        program->Attach(fragmentShader->get());
        program->Attach(geometryShader->get());
        program->Link();
        program->Use();
        
        programs.push_back(program);
        
        GLint vpLocation = glGetUniformLocation(program->get(), "VP");
        glm::mat4 VP = projection * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(VP));
        GLint modelsLocation = glGetUniformLocation(program->get(), "modelsMatrices");
        std::vector<glm::vec4> modelMatrices;
        modelMatrices.push_back(glm::vec4(-2.0f, -2.0f, 0.0f, 1.0f));
        modelMatrices.push_back(glm::vec4(2.0f, -2.0f, 0.0f, 1.0f));
        modelMatrices.push_back(glm::vec4(-2.0f, 2.0f, 0.0f, 1.0f));
        modelMatrices.push_back(glm::vec4(2.0f, 2.0f, 0.0f, 1.0f));
        glUniform4fv(modelsLocation, 4, glm::value_ptr(modelMatrices[0]));
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
    }
    return res;
}

void OGLInstancedDraw::Render(double time)
{
    glm::vec4 bgColor(1.0f, 0.79f, 0.32f, 1.0f);
    glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
    glDrawArraysInstanced(GL_POINTS, 0, 1, 4);
}
