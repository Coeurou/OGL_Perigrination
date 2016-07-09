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
#include "OGLPointSize.hpp"

OGLApplication::OGLApplication()
    : glfwApp(new OGLglfw), stage(new OGLPointSize)
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

	res = stage->Init(glfwApp->GetGLFWWindow().GetWindowWidth(), glfwApp->GetGLFWWindow().GetWindowHeight());

    return res;
}

int OGLApplication::Run()
{        
    auto loop = [&]() {
		stage->Render(glfwGetTime());
        TwDraw();
    };
    glfwApp->RunMainLoop(loop);
	return 0;
}
