#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <math.h>
#include "OGLApplication.hpp"
#include "GLFWUtility.hpp"

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

int OGLApplication::Run()
{
    
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' ");
    glm::vec4 bgColor(1.0f, 1.0f, 0.0f, 1.0f);
    TwAddVarRW(atbApp->GetBarByIndex(0), "cubeColor", TW_TYPE_COLOR4F, &bgColor,
               " label='Background color' help='Color and transparency of the background.' ");
    bool isDynamicBg = false;
    TwAddVarRW(atbApp->GetBarByIndex(0), "Has dynamic background", TW_TYPE_BOOLCPP, &isDynamicBg,
               " label='Dynamic Background' key=b help='Enable dynamic background through time.' ");
    
	auto loop = [&]() {
        if (isDynamicBg) {
            double time = glfwGetTime();
            bgColor.r = cos(time) * 0.5f + 0.5f;
            bgColor.g = sin(time) * 0.5f + 0.5f;
            bgColor.b = 0.5f;
        }
        glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
        TwDraw();
    };
    glfwApp->RunMainLoop(loop);
	return 0;
}
