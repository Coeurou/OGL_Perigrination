#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <functional>
#include <math.h>
#include "OGLApplication.hpp"
#include "GLFWUtility.hpp"
#include "OGLVertexArray.hpp"
#include "OGLProgram.hpp"
#include "OGLShader.hpp"

OGLApplication::OGLApplication()
    : glfwApp(new OGLglfw), atbApp(new OGLAntTweakBar)
{
}

OGLApplication::~OGLApplication()
{
}

bool OGLApplication::Init()
{
	bool res = glfwApp->InitGLFW();
    if (!res) {
        glfwApp = nullptr;
        return res;
    }
    glewExperimental = GL_TRUE;
	GLenum resGLEW = glewInit();
    res = (resGLEW == GLEW_OK);
	if (!res)
	{
		std::cerr << "Error: " << glewGetErrorString(resGLEW) << std::endl;
        return res;
	}
   
    res = atbApp->InitAntTweakBar("TweakBar", glfwApp->GetGLFWWindow().GetWindowWidth(),
                                  glfwApp->GetGLFWWindow().GetWindowHeight());
    if (!res)
    {
        std::cerr << "Error in AntTweakBar initialization" << std::endl;
        return res;
    }
    return res;
}

void OGLApplication::Render()
{
    
}

int OGLApplication::Run()
{
    Render();
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' ");
    glm::vec4 bgColor(1.0f, 1.0f, 0.0f, 1.0f);
    TwAddVarRW(atbApp->GetBarByIndex(0), "cubeColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    bool isDynamicBg = false;
    TwAddVarRW(atbApp->GetBarByIndex(0), "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    
    OGLVertexArray vao;
    vao.Bind();
    
    OGLProgram program;
    OGLShader vertexShader(GL_VERTEX_SHADER);
    OGLShader fragShader(GL_FRAGMENT_SHADER);
    vertexShader.SetSource("firstPoint.vert");
    vertexShader.Compile();
    fragShader.SetSource("firstPoint.frag");
    fragShader.Compile();
    program.Attach(vertexShader.get());
    program.Attach(fragShader.get());
    program.Link();
    program.Use();
    
    glPointSize(40);
    
    auto loop = [&]() {
        if (isDynamicBg) {
            float time = (float)glfwGetTime();
            bgColor.r = cosf(time) * 0.5f + 0.5f;
            bgColor.g = sinf(time) * 0.5f + 0.5f;
            bgColor.b = 0.5f;
        }
        glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
        glDrawArrays(GL_POINTS, 0, 1);
        TwDraw();
    };
    glfwApp->RunMainLoop(loop);
	return 0;
}
