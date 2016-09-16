//
//  OGLMultipleLights.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLMultipleLights.hpp"
#include <GL/glew.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "OGLUtility.hpp"
#include "Model.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLCube.hpp"

OGLMultipleLights::OGLMultipleLights()
{
}

OGLMultipleLights::~OGLMultipleLights()
{
    glDisable(GL_DEPTH_TEST);
}

bool OGLMultipleLights::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Multiple Lights' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLMultipleLights::AddProgram(const std::string & vsFilename, const std::string & fsFilename)
{
    bool res = true;
    auto program = std::make_shared<gs::Program>();
    res &= program->CreateShader(GL_VERTEX_SHADER, vsFilename);
    res &= program->CreateShader(GL_FRAGMENT_SHADER, fsFilename);
    
    res &= program->Link();
    program->Use();
    programs.push_back(program);
    return res;
}

void OGLMultipleLights::InitCubePosition()
{
    cubePositions[0] = glm::vec3(0);
    cubePositions[1] = glm::vec3(0, 20, 0);
    cubePositions[2] = glm::vec3(-10, 10, -12);
    cubePositions[3] = glm::vec3(-30, -7, -6);
    cubePositions[4] = glm::vec3(-28, -30, -10);
    cubePositions[5] = glm::vec3(-20, -12, -4);
    cubePositions[6] = glm::vec3(30, -12, -4);
    cubePositions[7] = glm::vec3(15, -10, -20);
    cubePositions[8] = glm::vec3(20, 0, -10);
    cubePositions[9] = glm::vec3(0, -20, -10);
}

void OGLMultipleLights::InitLight()
{
    dirLight.direction = glm::vec3(0, 0.5f, 0.5f);
    dirLight.ambientColor = glm::vec3(0.2f);
    dirLight.diffuseColor = glm::vec3(0.9f, 0.87f, 0.112f);
    dirLight.specularColor = glm::vec3(1.0f);
    
    spotLight.position = camera.GetPosition();
    spotLight.direction = camera.GetForwardDirection();
    spotLight.ambientColor = glm::vec3(0.2f);
    spotLight.diffuseColor = glm::vec3(0.8f);
    spotLight.specularColor = glm::vec3(1.0f);
    spotLight.innerCutOff = 12.5f;
    spotLight.outerCutOff = 17.5f;
    
    std::vector<glm::vec3> ptLightsPos { glm::vec3(0,10,0), glm::vec3(0,0,10), glm::vec3(10,0,0), glm::vec3(-10, 0,-5) };
    for (size_t i = 0; i < 4; i++) {
        ptLights[i].position = ptLightsPos[i];
        ptLights[i].ambientColor = glm::vec3(0.2f);
        ptLights[i].diffuseColor = glm::vec3(0.26f, 0.41f, 0.8f);
        ptLights[i].specularColor = glm::vec3(1.0f);
        ptLights[i].constantAttenuation = 1.0f;
        ptLights[i].linearAttenuation = 0.09f;
        ptLights[i].quadraticAttenuation = 0.032f;
    }
}

bool OGLMultipleLights::AddLightUniform(gs::Program* program)
{
    bool res = true;
    res &= program->AddUniform("dirLight.direction");
    res &= program->AddUniform("dirLight.ambientColor");
    res &= program->AddUniform("dirLight.diffuseColor");
    res &= program->AddUniform("dirLight.specularColor");
    for (int i = 0; i < 4; i++) {
        const std::string index = std::to_string(i);
        res &= program->AddUniform("ptLights[" + index + "].position");
        res &= program->AddUniform("ptLights[" + index + "].ambientColor");
        res &= program->AddUniform("ptLights[" + index + "].diffuseColor");
        res &= program->AddUniform("ptLights[" + index + "].specularColor");
        res &= program->AddUniform("ptLights[" + index + "].constantAttenuation");
        res &= program->AddUniform("ptLights[" + index + "].linearAttenuation");
        res &= program->AddUniform("ptLights[" + index + "].quadraticAttenuation");
    }
    res &= program->AddUniform("spotLight.position");
    res &= program->AddUniform("spotLight.direction");
    res &= program->AddUniform("spotLight.ambientColor");
    res &= program->AddUniform("spotLight.diffuseColor");
    res &= program->AddUniform("spotLight.specularColor");
    res &= program->AddUniform("spotLight.innerCutOff");
    res &= program->AddUniform("spotLight.outerCutOff");
    return res;
}

bool OGLMultipleLights::AddMatricesUniform(gs::Program * program)
{
    bool res = true;
    res &= program->AddUniform("MVP");
    res &= program->AddUniform("MV");
    res &= program->AddUniform("normalMatrix");
    return res;
}

bool OGLMultipleLights::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    if (!res) {
        return false;
    }
    
    cubePositions.resize(10);
    
    res &= InitGUI();
    
    // Init Camera
    camera.SetPosition(glm::vec3(0, 0, 50));
    camera.SetSpeed(15.0f);
    camera.SetupProjection(45.0f, windowWidth / (float)windowHeight);
    
    ptLights.resize(4);
    // Init light
    InitLight();
    
    // Init materials
    InitCubePosition();
    
    // Init program
    res &= AddProgram("mesh.vert", "multipleLights.frag");
    auto program = programs[0];
    
    // Get uniform locations
    res &= AddLightUniform(program.get());
    res &= AddMatricesUniform(program.get());
    res &= program->AddUniform("material.shininess");
    res &= program->AddUniform("samplerDiffuse1");
    program->AddUniform("samplerDiffuse2");
    program->AddUniform("samplerDiffuse3");
    program->AddUniform("samplerSpecular1");
    program->AddUniform("samplerSpecular2");
    
    glUniform1i(program->GetUniform("samplerDiffuse1"), 0);
    glUniform1i(program->GetUniform("samplerDiffuse2"), 1);
    glUniform1i(program->GetUniform("samplerDiffuse3"), 2);
    glUniform1i(program->GetUniform("samplerSpecular1"), 3);
    glUniform1i(program->GetUniform("samplerSpecular2"), 4);
    
    // Init geometry
    auto vao = std::make_shared<gs::VertexArray>();
    vaos.push_back(vao);
    vao->BindVAO();
    
    auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
    vbos.push_back(vbo);
    vbo->BindVBO();
    OGLCube cube;
    auto& vertices = cube.GetVertices();
    cube.InitVertices(glm::vec3(0));
    glBufferData(vbo->GetTarget(), sizeof(gs::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    vao->AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), 0);
    vao->AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
    vao->AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));
    
    auto diffuse = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
    diffuse->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
    res &= diffuse->LoadTexture("containerDiffuse.dds");
    diffuse->BindTexture(GL_TEXTURE0);
    textures.push_back(diffuse);
    
    auto specular = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
    specular->SetContribution(LIGHT_CONTRIBUTION::DIFFUSE);
    res &= specular->LoadTexture("containerSpecular.dds");
    specular->BindTexture(GL_TEXTURE3);
    textures.push_back(specular);
    res=true;
    glEnable(GL_DEPTH_TEST);
    
    return res;
}

void OGLMultipleLights::SendLightUniform(const gs::Program* program)
{
    glm::vec4 lightDir = camera.GetViewMatrix() * glm::vec4(dirLight.direction, 0.0f);
    glUniform3fv(program->GetUniform("dirLight.direction"), 1, glm::value_ptr(glm::normalize(lightDir)));
    glUniform3fv(program->GetUniform("dirLight.ambientColor"), 1, glm::value_ptr(dirLight.ambientColor));
    glUniform3fv(program->GetUniform("dirLight.diffuseColor"), 1, glm::value_ptr(dirLight.diffuseColor));
    glUniform3fv(program->GetUniform("dirLight.specularColor"), 1, glm::value_ptr(dirLight.specularColor));

    for (size_t i = 0; i < 4; i++) {
        const std::string index = std::to_string(i);
        glm::vec4 lightPos = camera.GetViewMatrix() * glm::vec4(ptLights[i].position, 1.0f);
        glUniform3fv(program->GetUniform("ptLights[" + index + "].position"), 1, glm::value_ptr(lightPos));
        glUniform3fv(program->GetUniform("ptLights[" + index + "].ambientColor"), 1, glm::value_ptr(ptLights[i].ambientColor));
        glUniform3fv(program->GetUniform("ptLights[" + index + "].diffuseColor"), 1, glm::value_ptr(ptLights[i].diffuseColor));
        glUniform3fv(program->GetUniform("ptLights[" + index + "].specularColor"), 1, glm::value_ptr(ptLights[i].specularColor));
        glUniform1f(program->GetUniform("ptLights[" + index + "].constantAttenuation"), ptLights[i].constantAttenuation);
        glUniform1f(program->GetUniform("ptLights[" + index + "].linearAttenuation"), ptLights[i].linearAttenuation);
        glUniform1f(program->GetUniform("ptLights[" + index + "].quadraticAttenuation"), ptLights[i].quadraticAttenuation);
    }
    
    glUniform3fv(program->GetUniform("spotLight.position"), 1, glm::value_ptr(spotLight.position));
    lightDir = camera.GetViewMatrix() * glm::vec4(spotLight.direction, 0.0f);
    glUniform3fv(program->GetUniform("spotLight.direction"), 1, glm::value_ptr(glm::normalize(lightDir)));
    glUniform3fv(program->GetUniform("spotLight.ambientColor"), 1, glm::value_ptr(spotLight.ambientColor));
    glUniform3fv(program->GetUniform("spotLight.diffuseColor"), 1, glm::value_ptr(spotLight.diffuseColor));
    glUniform3fv(program->GetUniform("spotLight.specularColor"), 1, glm::value_ptr(spotLight.specularColor));
    glUniform1f(program->GetUniform("spotLight.innerCutOff"), cosf(glm::radians(spotLight.innerCutOff)));
    glUniform1f(program->GetUniform("spotLight.outerCutOff"), cosf(glm::radians(spotLight.outerCutOff)));
    
    
}

void OGLMultipleLights::SendMatricesUniform(const gs::Program* program, const glm::mat4& model)
{
    glUniformMatrix4fv(program->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
    glUniformMatrix4fv(program->GetUniform("MV"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix() * model));
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(camera.GetViewMatrix() * model));
    glUniformMatrix4fv(program->GetUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void OGLMultipleLights::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glm::vec3 bgColor(0.2f);
    glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
    
    camera.Update();
    spotLight.position = camera.GetPosition();
    spotLight.direction = camera.GetForwardDirection();
    
    // Render container
    auto programMesh = programs[0];
    programMesh->Use();
    
    SendLightUniform(programMesh.get());
    
    glm::mat4 model = glm::mat4(1);
    for (size_t i = 0; i < 10; i++) {
        float sign = (i % 2) * 2 - 1;
        model = glm::mat4(1);
        model = glm::rotate(model, i * 0.2f * glm::two_pi<float>(), glm::vec3(0, 1, 0)) *
        glm::translate(model, glm::vec3(20, 0, 20));
        
        model = glm::scale(glm::rotate(glm::translate(glm::mat4(1), cubePositions[i]), glm::radians(sign * 30.0f), cubePositions[i]), glm::vec3(10));
        SendMatricesUniform(programMesh.get(), model);
        vaos[0]->BindVAO();
        vbos[0]->BindVBO();
        textures[0]->BindTexture(GL_TEXTURE0);
        textures[1]->BindTexture(GL_TEXTURE3);
        glUniform1f(programMesh->GetUniform("material.shininess"), 32.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
