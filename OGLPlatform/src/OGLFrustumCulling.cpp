//
//  OGLFrustumCulling.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLFrustumCulling.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iostream>

OGLFrustumCulling::OGLFrustumCulling() : frustumComputer(&camera), primitiveQuery(GL_PRIMITIVES_GENERATED)
{
}

OGLFrustumCulling::~OGLFrustumCulling()
{
}

bool OGLFrustumCulling::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Frustum Culling Example' ";
    
    TwAddVarRW(tweakBar, "waveAmpl", TW_TYPE_FLOAT, &waveAmplitude, "label='Wave Amplitude' step='0.5'");
    TwAddVarRW(tweakBar, "useExtCam", TW_TYPE_BOOLCPP, &useExternalCamera, "label='External camera'");
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLFrustumCulling::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();

        auto program = std::make_shared<gs::Program>();
        program->CreateShader(GL_VERTEX_SHADER, "sinDisplacement.vert");
        program->CreateShader(GL_GEOMETRY_SHADER, "frustumCulling.geom");
        program->CreateShader(GL_FRAGMENT_SHADER, "roundedPoints.frag");

        res &= program->Link();
        program->Use();
        programs.push_back(program);
        
        program->AddUniform("MVP");
        program->AddUniform("time");
        program->AddUniform("frustumPlanes");
        program->AddUniform("amplitude");
        
        auto programFrustum = std::make_shared<gs::Program>();
        programFrustum->CreateShader(GL_VERTEX_SHADER, "oglLight.vert");
        programFrustum->CreateShader(GL_FRAGMENT_SHADER, "firstPoint.frag");
        
        res &= programFrustum->Link();
        programFrustum->Use();
        programs.push_back(programFrustum);
        
        programFrustum->AddUniform("MVP");
        
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
        auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
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
        vbos.push_back(vbo);
        
        vao->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        totalVertices = (int)positions.size();
        
        ground.SetSize(1000);
		ground.SetProgram(programs[1]);
        ground.Load("");
        
        camera.SetSpeed(10);
        camera.SetPosition(glm::vec3(0,0,20));
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight, 0.01f);
        
        cameraExternal.SetSpeed(0);
        cameraExternal.SetPosition(glm::vec3(0,20,30));
        cameraExternal.SetTarget(camera.GetPosition());
        cameraExternal.SetupProjection(45.0f, windowWidth/(float)windowHeight);
        cameraExternal.Update();
        
        glPointSize(30.0f);
        //glCullFace(GL_FRONT_FACE);
    }
    return res;
}

void OGLFrustumCulling::Render(double time)
{
    if (!useExternalCamera)
        camera.Update();
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)));
    
    programs[1]->Use();
    glm::mat4 makeHorizontal = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1,0,0));
    glUniformMatrix4fv(programs[1]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * makeHorizontal));
    ground.Render();
    
    programs[0]->Use();
    vaos[0]->BindVAO();
    vbos[0]->BindVBO();
    glm::mat4 mvp = (useExternalCamera) ? cameraExternal.GetViewProjectionMatrix() : camera.GetViewProjectionMatrix();
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    
    glUniform1f(programs[0]->GetUniform("time"), (float)time);
    
    glUniform1f(programs[0]->GetUniform("amplitude"), waveAmplitude);
    
    auto planes = frustumComputer.GetFrustumPlanes();
    glUniform4fv(programs[0]->GetUniform("frustumPlanes"), 6, glm::value_ptr(planes[0]));
    
    primitiveQuery.BeginQuery();
    glDrawArrays(GL_POINTS, 0, totalVertices);
    primitiveQuery.EndQuery();
    
    GLuint nbPoints = 0;
    glGetQueryObjectuiv(primitiveQuery.get(), GL_QUERY_RESULT, &nbPoints);
    float cullPercentage = 100.0f - (nbPoints/(float)totalVertices * 100.0f);
    std::cout << "Culling percentage: " << cullPercentage << std::endl;
}
