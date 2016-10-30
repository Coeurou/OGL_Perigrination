//
//  OGLColorPicking.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 16/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLColorPicking.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "OGLUtility.hpp"
#include "EventManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/epsilon.hpp>
#include <sstream>

OGLColorPicking::OGLColorPicking()
{
	gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_PRESSED, this);
}

OGLColorPicking::~OGLColorPicking()
{
    glDisable(GL_DEPTH_TEST);
	gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_PRESSED, this);
}

bool OGLColorPicking::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " << " label='Frustum Culling Example' ";
    
    TwDefine(format.str().c_str());
    
    return true;
}

bool OGLColorPicking::Init(int windowWidth, int windowHeight)
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

		camera.SetSpeed(10);
        camera.SetPosition(glm::vec3(45,15,10));
        camera.SetupProjection(45.0f, windowWidth/(float)windowHeight, 0.1f, 1000.0f);
        glEnable(GL_DEPTH_TEST);
    }
	return res;
}

void OGLColorPicking::InitGround(size_t xCount, size_t zCount, size_t worldSizeX, size_t worldSizeZ)
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

void OGLColorPicking::Render(double time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
	glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));

    camera.Update();
    
    glm::mat4 viewProjMatrix = camera.GetViewProjectionMatrix();

    for (size_t r = 0; r < 150; r += 15) {
        for (size_t g = 0; g < 150; g += 15) {
            for (size_t b = 0; b < 150; b += 15) {
                color = glm::vec4(r/255.0f,g/255.0f,b/255.0f,1.0f);
                auto isEqual = glm::epsilonEqual(color, selectedColor, 0.01f);
                color = (isEqual.x && isEqual.y && isEqual.z) ? glm::vec4(1.0f) : color;
                glm::vec3 translation(r/8.0f, g/8.0f, b/8.0f);
                glm::mat4 model = glm::translate(glm::mat4(1), translation);
                
                glUniformMatrix4fv(programs[0]->GetUniform("MVP"), 1, GL_FALSE, glm::value_ptr(viewProjMatrix*model));
                glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
                cube.Render();
            }
        }
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
	glUniform4fv(programs[0]->GetUniform("color"), 1, glm::value_ptr(color));
	glDrawElements(GL_LINES, 400, GL_UNSIGNED_SHORT, nullptr);
	glLineWidth(size);
}

void OGLColorPicking::OnMouseButtonDown(gs::Event e)
{
	glm::ivec4 viewport;
	glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
	int x = (int)e.args.mousePosX;
	int y = (int)e.args.mousePosY;
    glm::u8vec4 pixelColor;
	glReadPixels(x,viewport.w-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixelColor);
    selectedColor = glm::vec4(pixelColor) / 255.0f;
}

void OGLColorPicking::OnEvent(gs::Event e)
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
