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
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLCube.hpp"
#include "OGLQuad.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLCameraStage::OGLCameraStage()
{}

OGLCameraStage::~OGLCameraStage()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

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

		// Program textured objects setup
        auto vertexShader = std::make_unique<gs::Shader>(GL_VERTEX_SHADER);
        auto programTex = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("simpleCube.vert");
        res &= vertexShader->Compile();
        programTex->Attach(vertexShader->get());
        
        auto fragmentShader = std::make_unique<gs::Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSource("simpleCube.frag");
        res &= fragmentShader->Compile();
        programTex->Attach(fragmentShader->get());
        
		res &= programTex->Link();
		programTex->Use();
        programs.push_back(programTex);

		mvpLocationTex = glGetUniformLocation(programTex->get(), "MVP");

		// Program blended objects setup
		auto programBlend = std::make_shared<gs::Program>();
		programBlend->Attach(vertexShader->get());

		auto fragmentShaderBlend = std::make_unique<gs::Shader>(GL_FRAGMENT_SHADER);
		fragmentShaderBlend->SetSource("simpleBlending.frag");
		res &= fragmentShaderBlend->Compile();
		programBlend->Attach(fragmentShaderBlend->get());

		res &= programBlend->Link();
		programBlend->Use();
		programs.push_back(programBlend);
        
		mvpLocationBlend = glGetUniformLocation(programBlend->get(), "MVP");
		uColorLocation = glGetUniformLocation(programBlend->get(), "uColor");
		blendColors[0] = glm::vec4(0.576f, 0.8f, 0.898f, 0.5f);
		blendColors[1] = glm::vec4(0.91f, 0.79f, 0.37f, 0.5f);
		blendColors[2] = glm::vec4(0.71f, 0.215f, 0.71f, 0.5f);
		blendColors[3] = glm::vec4(0.466f, 0.925f, 0.854f, 0.5f);
		blendColors[4] = glm::vec4(0.71f, 0.215f, 0.215, 0.5f);
        
		//Texture setup
		auto textureFloor = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
		res &= textureFloor->LoadTexture("Floor.dds");
		textureFloor->ChangeParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		textureFloor->ChangeParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		textures.push_back(std::move(textureFloor));

		auto textureCamouflage = std::make_unique<gs::Texture>(IMAGE_TYPE::GLI);
		res &= textureCamouflage->LoadTexture("Camouflage.dds");
		textures.push_back(std::move(textureCamouflage));

		// Geometry setup
        auto vao = std::make_unique<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(std::move(vao));
        
		auto vbo = std::make_unique<gs::VertexBuffer>(GL_ARRAY_BUFFER);
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

		vbos.push_back(std::move(vbo));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gs::Vertex), (void*)offsetof(gs::Vertex, texCoords));

		// Camera setup
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        camera.SetSpeed(12.0f);
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);

		// OpenGL setup
		glEnable(GL_DEPTH_TEST);

    }
    return res;
}

void OGLCameraStage::Render(double time)
{
    glm::vec4 bgColor(1.0f, 0.76f, 0.42f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
    
	camera.Update();

	programs[0]->Use();
	textures[0]->BindTexture(GL_TEXTURE0);

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f));

	glUniformMatrix4fv(mvpLocationTex, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
    glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);

	textures[1]->BindTexture(GL_TEXTURE0);
	for (int i = 0; i < NB_CUBE; i++)
	{
		float sign = (i % 2) * 2.0f - 1.0f;
		glm::vec3 offset = glm::vec3(sign * 8.0f, 2.0f, -100 + i * 25.0f);
		model = glm::translate(glm::mat4(1.0f), offset);
		model = glm::rotate(model, glm::radians(45.0f * (float)time), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		glUniformMatrix4fv(mvpLocationTex, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	programs[1]->Use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);
	for (int i = 0; i < NB_CUBE; i++)
	{
		float sign = (i % 2) * 2.0f - 1.0f;
		glm::vec3 offset = glm::vec3(-sign * 8.0f, 2.0f, -100 + i * 25.0f);
		model = glm::translate(glm::mat4(1.0f), offset);
		model = glm::rotate(model, glm::radians(45.0f * (float)time), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		glUniformMatrix4fv(mvpLocationBlend, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix() * model));
		glUniform4fv(uColorLocation, 1, glm::value_ptr(blendColors[i]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glDisable(GL_BLEND);
	glDepthMask(1);
}
