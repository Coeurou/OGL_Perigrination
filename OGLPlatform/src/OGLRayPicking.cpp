//
//  OGLRayPicking.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLRayPicking.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "Ray.hpp"
#include "RayUtils.hpp"
#include "OGLUtility.hpp"
#include "EventManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

OGLRayPicking::OGLRayPicking()
{
	gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_PRESSED, this);
}

OGLRayPicking::~OGLRayPicking()
{
    glDisable(GL_DEPTH_TEST);
	gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_PRESSED, this);
}

bool OGLRayPicking::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Frustum Culling Example' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLRayPicking::Init(int windowWidth, int windowHeight)
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
        
		cube.SetProgram(programs[0]);
        cube.Load("");

		InitGround(NBVERTICESX, NBVERTICESZ, WORLDSIZEX, WORLDSIZEZ);

		int offset = 2;
		for (size_t i = 0; i < NBBOXESX; i++) {	
			for (size_t j = 0; j < NBBOXESY; j++) {
				for (size_t k = 0; k < NBBOXESZ; k++) {
					float sign = (i % 2) * 2 - 1.0f;
					float z = (k % 2 == 0) ? -3 : 0;
					boxPositions.push_back(glm::vec3(offset * i * sign, offset * j, z));
					gs::AABB box;
					box.min = boxPositions.back() - 0.5f;
					box.max = boxPositions.back() + 0.5f;
					boxes.push_back(box);
				}
			}
		}

		camera.SetPosition(glm::vec3(0,10,30));
		camera.SetSpeed(10);
		camera.SetupProjection(45.0f, windowWidth/(float)windowHeight, 0.1f, 1000.0f);
        glEnable(GL_DEPTH_TEST);
    }
	return res;
}

void OGLRayPicking::InitGround(size_t xCount, size_t zCount, size_t worldSizeX, size_t worldSizeZ)
{
	std::vector<glm::vec3> vertices(xCount * zCount);
	float halfWorldSizeX = worldSizeX * 0.5f;
	float halfWorldSizeZ = worldSizeZ * 0.5f;

	// Create a plane ok X / Z vertices and a size of WORLD_SIZE
	for (size_t z = 0; z < zCount; z++) {
		for (size_t x = 0; x < xCount; x++) {
			glm::vec3 vertex(0.0f);
			vertex.x = ((x/(float)xCount) * 2 - 1.0f) * halfWorldSizeX;
			vertex.y = -0.75f;
			vertex.z = ((z/(float)zCount) * 2 - 1.0f) * halfWorldSizeZ;
			vertices[xCount*z+x] = vertex;
		}
	}
	std::vector<GLushort> indices;
	for (size_t h = 0; h < zCount; h++) {
		indices.push_back((GLushort)(h*zCount));
		indices.push_back((GLushort)((h+1)*(zCount) - 1));
	}
	for (size_t v = 0; v < xCount; v++) {
		indices.push_back((GLushort)v);
		indices.push_back((GLushort)((xCount-1) * zCount + v));
	}
	ground.vao.BindVAO();

	ground.vbo.BindVBO();
	glBufferData(ground.vbo.GetTarget(), vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	ground.ibo.BindVBO();
	glBufferData(ground.ibo.GetTarget(), indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	ground.vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void OGLRayPicking::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
	glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));

    camera.Update();
    
    glm::mat4 viewProjMatrix = camera.GetViewProjectionMatrix();
	
	for (int i = 0; i < boxPositions.size(); i++) {
		auto pos = boxPositions[i];
		color = (i == selectedIndex) ? glm::vec4(1) : glm::vec4((pos.x*8.0f) / 255.0f, (pos.y*8.0f) / 255.0f, 0.21f, 1.0f);
		float sign = (i % 2) * 2 - 1.0f;
        glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1), pos), glm::radians(35.0f*(float)time*sign), glm::vec3(0.1f, 1.0f, 0.6f));
                
        glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(viewProjMatrix*model));
        glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
        cube.Render();
    }
    
    glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(viewProjMatrix));
	color = glm::vec4(0.8f);
	glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
	
	float size = 0;
	glGetFloatv(GL_LINE_WIDTH, &size);
	glLineWidth(2.0f);
	ground.vao.BindVAO();
	ground.vbo.BindVBO();
	ground.ibo.BindVBO();
	glDrawElements(GL_LINES, 400, GL_UNSIGNED_SHORT, nullptr);
	glLineWidth(size);
}

void OGLRayPicking::OnMouseButtonDown(gs::Event e)
{
	glm::ivec4 viewport;
	glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
	int x = (int)e.args.mousePosX;
	int y = (int)e.args.mousePosY;
	
	gs::Ray ray;
	ray.origin = camera.GetPosition();
	glm::vec3 start = glm::unProject(glm::vec3(x, viewport.w - y, 0), camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);
	glm::vec3 end = glm::unProject(glm::vec3(x, viewport.w - y, 1), camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);
	ray.direction = glm::normalize(end - start);

	float minDist = std::numeric_limits<float>::max();
	for (size_t i = 0; i < boxes.size(); i++) {
		auto& box = boxes[i];
		auto intersection = IntersectBox(ray, box);
		if (intersection.x < intersection.y && intersection.x < minDist) {
			selectedIndex = (int)i;
			minDist = intersection.x;
		}
	}
}

void OGLRayPicking::OnEvent(gs::Event e)
{
	switch (e.GetEventType())
	{
	case EventType::ET_MOUSE_PRESSED:
            if (e.args.mouseButton == (int)MouseButton::MB_LEFT && e.args.mouseButtonState == (int)ButtonState::BUTTON_PRESSED) {
			OnMouseButtonDown(e);
		}
		break;
	default:
		break;
	}
}
