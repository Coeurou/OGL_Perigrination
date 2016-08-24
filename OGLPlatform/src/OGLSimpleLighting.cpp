//
//  OGLSimpleLighting.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 15/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLSimpleLighting.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLCube.hpp"
#include "OGLQuad.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLSimpleLighting::OGLSimpleLighting()
{}

OGLSimpleLighting::~OGLSimpleLighting()
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

bool OGLSimpleLighting::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Free camera Example' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLSimpleLighting::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();

		// Program textured objects setup
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto programTex = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("simpleLighting.vert");
        res &= vertexShader->Compile();
        programTex->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("simpleLighting.frag");
        res &= fragmentShader->Compile();
        programTex->Attach(fragmentShader->get());
        
        programTex->Link();
		programTex->Use();
        programs.push_back(programTex);

		mvpLocation = glGetUniformLocation(programTex->get(), "MVP");
		normalMatrixLocation = glGetUniformLocation(programTex->get(), "NormalMatrix");
		light.color = glm::vec3(1.0f);
		light.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		light.ambientIntensity = 0.2f;
		light.diffuseIntensity = 0.8f;

		GLint lightColorLoc = glGetUniformLocation(programTex->get(), "light.color");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.color));
		GLint lightDirLoc = glGetUniformLocation(programTex->get(), "light.direction");
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(light.direction));
		GLint ambientLoc = glGetUniformLocation(programTex->get(), "light.ambientIntensity");
		glUniform1f(ambientLoc, light.ambientIntensity);
		GLint diffuseLoc = glGetUniformLocation(programTex->get(), "light.diffuseIntensity");
		glUniform1f(diffuseLoc, light.diffuseIntensity);
        
		//Texture setup
		auto textureFloor = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
		res &= textureFloor->LoadTexture("Floor.dds");
		textureFloor->ChangeParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		textureFloor->ChangeParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		textures.push_back(textureFloor);

		auto textureCamouflage = std::make_shared<gs::Texture>(IMAGE_TYPE::GLI);
		res &= textureCamouflage->LoadTexture("Hieroglyphes.dds");
		textures.push_back(textureCamouflage);

		// Geometry setup
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);
        
		auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
		vbo->BindVBO();

		std::vector<gs::Vertex> vertices;
		OGLCube cube;
		cube.InitVertices(glm::vec3(0));
		vertices.insert(vertices.end(), cube.GetVertices().begin(), cube.GetVertices().end());		

		OGLQuad quad;
		quad.SetSize(1000);
		quad.InitVertices(glm::vec3(0));
		vertices.insert(vertices.end(), quad.GetVertices().begin(), quad.GetVertices().end());

		glBufferData(GL_ARRAY_BUFFER, sizeof(gs::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		vbos.push_back(vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, normal));

		// Camera setup
        camera.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
        camera.SetSpeed(12.0f);
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);

		// OpenGL setup
		glEnable(GL_DEPTH_TEST);
    }
    return res;
}

void OGLSimpleLighting::Render(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	camera.Update();

	textures[0]->BindTexture(GL_TEXTURE0);

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f));

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
    glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);

	textures[1]->BindTexture(GL_TEXTURE0);
	for (int i = 0; i < NB_CUBE; i++)
	{
		float sign = (i % 2) * 2.0f - 1.0f;
		glm::vec3 offset = glm::vec3(sign * 8.0f, 2.0f, 100 - i * 25.0f);
		model = glm::translate(glm::mat4(1.0f), offset);
		model = glm::rotate(model, glm::radians(45.0f * (float)time), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));	
		model = glm::inverse(glm::transpose(model));
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
