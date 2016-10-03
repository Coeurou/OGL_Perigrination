//
//  WindowGLFW.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "WindowGLFW.hpp"
#include "GLFWContext.hpp"
#include "GLFWUtility.hpp"
#include <iostream>

WindowGLFW::WindowGLFW()
{
    context = nullptr;
}

WindowGLFW::WindowGLFW(gs::Context* ctxt)
{
    context = std::shared_ptr<gs::Context>(ctxt);
}

WindowGLFW::~WindowGLFW()
{
	glfwDestroyWindow(window);
}

bool WindowGLFW::CreateWindow()
{
    bool res = context->InitContext();
    auto params = context->GetContextParams();
    window = glfwCreateWindow(params.windowWidth, params.windowHeight, title.c_str(), nullptr, nullptr);
	res = (window != nullptr);

    if (res) {
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwMakeContextCurrent(window);
    }
	InitCallbacks();
    return res;
}

bool WindowGLFW::Render()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
	return (glfwWindowShouldClose(window) == 0);
}

void WindowGLFW::InitCallbacks()
{
	glfwSetErrorCallback(OnGLFWError);
	glfwSetKeyCallback(window, OnKeyDown);
	glfwSetCharCallback(window, OnCharPressed);
	glfwSetScrollCallback(window, OnMouseWheel);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetWindowSizeCallback(window, OnWindowResize);
	glfwSetMouseButtonCallback(window, OnMouseButtonClick);
}
