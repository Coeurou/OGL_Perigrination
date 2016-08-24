//
//  OGLIndexedDraw.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLIndexedDraw.hpp"
#include "OGLPoint.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "OGLUtility.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

OGLIndexedDraw::OGLIndexedDraw()
{}

OGLIndexedDraw::~OGLIndexedDraw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_PRIMITIVE_RESTART);
	glDisableVertexAttribArray(0);
}

bool OGLIndexedDraw::InitGUI()
{
    auto tweakBar = atbApp->GetBarByIndex(0);
    auto barName = TwGetBarName(tweakBar);
    std::stringstream format;
    format << barName << " " <<  " label='Draw instanced Example' help='This example use instance IDs & billboarding.' ";
	TwAddVarRW(tweakBar, "Div", TW_TYPE_FLOAT, &heightDivider, " label='Height divider' min='1' step='0.5' ");

    return true;
}

bool OGLIndexedDraw::Init(int windowWidth, int windowHeight)
{
    bool res = gs::Stage::Init(windowWidth, windowHeight);
    
    if (res) {
        res &= InitGUI();
        
		/******************************* Shaders ********************************/
		
        auto vertexShader = std::make_shared<gs::Shader>(GL_VERTEX_SHADER);
        auto fragmentShader = std::make_shared<gs::Shader>(GL_FRAGMENT_SHADER);
        auto program = std::make_shared<gs::Program>();
        
        vertexShader->SetSource("heightmap.vert");
        res &= vertexShader->Compile();
        fragmentShader->SetSource("heightmap.frag");
        res &= fragmentShader->Compile();
        
        program->Attach(vertexShader->get());
        program->Attach(fragmentShader->get());
        program->Link();
        program->Use();
        
        programs.push_back(program);
        
        glm::mat4 MVP = projection * glm::lookAt(glm::vec3(0, 40, -30), glm::vec3(0), glm::vec3(0, 1, 0));
        GLint mvpLocation = glGetUniformLocation(program->get(), "MVP");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));	

		hDivLocation = glGetUniformLocation(program->get(), "heightDivider");
        
		/******************************* Geometry ********************************/
        auto vao = std::make_shared<gs::VertexArray>();
        vao->BindVAO();
        vaos.push_back(vao);

		auto vbo = std::make_shared<gs::VertexBuffer>(GL_ARRAY_BUFFER);
		vbo->BindVBO();
		vbos.push_back(vbo);

		heights = { 4.0f, 2.0f, 3.0f, 1.0f,
					3.0f, 5.0f, 8.0f, 2.0f,
					7.0f, 10.0f, 12.0f, 6.0f,
					4.0f, 6.0f, 8.0f, 3.0f };

		const float halfX = WORLD_SIZE_X * 0.5f;
		const float halfZ = WORLD_SIZE_Z * 0.5f;

		for (size_t i = 0; i < HM_SIZE_Z; i++) {
			for (size_t j = 0; j < HM_SIZE_X; j++)
			{
				short currentLineOffset = (short)j * HM_SIZE_Z;

				float xPos = j / (float)(HM_SIZE_X - 1) * WORLD_SIZE_X - halfX;
				float yPos = heights[i + currentLineOffset];
				float zPos = i / (float)(HM_SIZE_Z - 1) * WORLD_SIZE_Z - halfZ;
				vertices[i + currentLineOffset] = glm::vec3(xPos, yPos, zPos);
			}
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		/******************************* Indices ********************************/
		auto ibo = std::make_shared<gs::VertexBuffer>(GL_ELEMENT_ARRAY_BUFFER);
		ibo->BindVBO();
		vbos.push_back(ibo);

		const GLushort restartIndex = HM_SIZE_X * HM_SIZE_Z;
		indices = { 0, 4, 1, 5, 2, 6, 3, 7, restartIndex,
					4, 8, 5, 9, 6, 10, 7, 11, restartIndex,
					8, 12, 9, 13, 10, 14, 11, 15};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnable(GL_PRIMITIVE_RESTART);
		glEnable(GL_DEPTH_TEST);
		glPrimitiveRestartIndex(restartIndex);
    }
    return res;
}

void OGLIndexedDraw::Render(double time)
{
    glm::vec4 bgColor(0.32f, 0.61f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
    glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

	glUniform1f(hDivLocation, heightDivider);

    glDrawElements(GL_TRIANGLE_STRIP, (int)indices.size(), GL_UNSIGNED_SHORT, nullptr);
}
