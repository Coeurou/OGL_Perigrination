//
//  OGLDepthPicking.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLDepthPicking.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLDepthPicking::OGLDepthPicking()
{
}

OGLDepthPicking::~OGLDepthPicking()
{
    glDisable(GL_DEPTH_TEST);
}

bool OGLDepthPicking::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Frustum Culling Example' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLDepthPicking::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();
        
        auto program = std::make_shared<gs::Program>();
        program->CreateShader(GL_VERTEX_SHADER, "oglLight.vert");
        program->CreateShader(GL_FRAGMENT_SHADER, "firstPoint.frag");
        
        res &= program->Link();
        program->Use();
        programs.push_back(program);
        
        program->AddUniform("MVP");
        
        ground.SetSize(1000);
		ground.SetProgram(programs[0]);
        ground.Load("");
        
        auto cube = std::make_unique<OGLRenderableCube>();
		cube->SetProgram(programs[0]);
        cube->Load("");
        //cube.SetSize(30);
        cubes.push_back(std::move(cube));
        
        camera.SetSpeed(10);
        camera.SetPosition(glm::vec3(0,0,20));
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);
        glEnable(GL_DEPTH_TEST);
    }
    return res;
}

void OGLDepthPicking::Render(double time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.Update();
    
    glm::mat4 mvp = camera.GetViewProjectionMatrix();
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    cubes[0]->Render();
    
    glm::mat4 makeHorizontal = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1,0,0));
    mvp *= makeHorizontal;
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    ground.Render();
}
