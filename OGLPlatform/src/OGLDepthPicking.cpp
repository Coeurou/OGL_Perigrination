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
#include "EventManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLDepthPicking::OGLDepthPicking()
{
	gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_PRESSED, this);
}

OGLDepthPicking::~OGLDepthPicking()
{
    glDisable(GL_DEPTH_TEST);
	gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_PRESSED, this);
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
        res &= program->CreateShader(GL_VERTEX_SHADER, "oglLight.vert");
		res &= program->CreateShader(GL_FRAGMENT_SHADER, "uniformColor.frag");
        
        res &= program->Link();
        program->Use();
        programs.push_back(program);
        
		program->AddUniform("MVP");
		program->AddUniform("color");
        
        ground.SetSize(1000);
		ground.SetProgram(programs[0]);
        ground.Load("");
        
		cube.SetProgram(programs[0]);
        cube.Load("");
        //cube.SetSize(30);
		for (size_t i = 1; i < 7; i++)
		{
			size_t j = (i % 2 == 1) ? i : i - 1;
			float sign = (i % 2) * 2 - 1.0f;
			boxPositions.push_back(glm::vec3(2 * sign*j, 0, 0));
		}

		camera.SetSpeed(0);
		camera.SetAngularSpeed(0);
		camera.SetPosition(glm::vec3(0, 2, 10));
		camera.SetTarget(glm::vec3(0,2,9));
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight);
        glEnable(GL_DEPTH_TEST);
    }
	return res;
}

void OGLDepthPicking::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
	glm::vec4 color(0.874f, 0.802f, 0.241f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));

    camera.Update();
    
    glm::mat4 viewProjMatrix = camera.GetViewProjectionMatrix();

	for (size_t i = 0; i < 6; i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(1), boxPositions[i]);

		glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(viewProjMatrix*model));
		color = (selectedBox == i) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
		cube.Render();
	}
    
    glm::mat4 makeHorizontal = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1,0,0));
    viewProjMatrix *= makeHorizontal;
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(viewProjMatrix));
	color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
	ground.Render();
}

void OGLDepthPicking::OnMouseButtonDown(gs::Event e)
{
	glm::ivec4 viewport;
	glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
	int x = (int)e.args.mousePosX;
	int y = (int)e.args.mousePosY;
	float z = 0.0f;
	glReadPixels(x, viewport.w - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	glm::vec3 ptPixel = glm::unProject(glm::vec3(x, viewport .w-y,z), camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);

	float minDist = 1000.0f;

	for (size_t i = 0; i < boxPositions.size(); i++) {
		float dist = glm::distance(ptPixel, boxPositions[i]);
		selectedBox = (dist < minDist) ? (int)i : selectedBox;
		minDist = std::min(dist, minDist);
	}
}

void OGLDepthPicking::OnEvent(gs::Event e)
{
	switch (e.GetEventType())
	{
	case EventType::ET_MOUSE_PRESSED:
		if (e.args.mouseButton == (int)MouseButton::MB_LEFT) {
			OnMouseButtonDown(e);
		}
		break;
	default:
		break;
	}
}
