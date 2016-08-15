//
//  OGLCameraStage.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 15/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLCameraStage.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLCameraStage::OGLCameraStage()
{}

OGLCameraStage::~OGLCameraStage()
{}

bool OGLCameraStage::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Free camera Example' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLCameraStage::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("simplePlane.vert");
        res &= vertexShader->Compile();
        program->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("simpleTriangle.frag");
        res &= fragmentShader->Compile();
        program->Attach(fragmentShader->get());
        
        program->Link();
        program->Use();
        programs.push_back(program);
        
        mvpLocation = glGetUniformLocation(program->get(), "MVP");
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        camera.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
        //camera.Rotate(glm::vec3(glm::radians(40.0f), 0.0f, 0.0f));
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);
    }
    return res;
}

void OGLCameraStage::Render(double time)
{
    glm::vec4 bgColor(1.0f, 0.76f, 0.42f, 1.0f);
    glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
    
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
