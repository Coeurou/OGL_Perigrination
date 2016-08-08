//
//  WindowGLFW.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "WindowGLFW.hpp"
#include "GLFWContext.hpp"
#include <iostream>

WindowGLFW::WindowGLFW()
{
    context = nullptr;
}

WindowGLFW::WindowGLFW(GLFWContext* ctxt)
{
    context = std::shared_ptr<GLFWContext>(ctxt);
}

WindowGLFW::~WindowGLFW()
{
	glfwDestroyWindow(window);
}

bool WindowGLFW::CreateWindow()
{
    bool res = false;
    auto params = context->GetContextParams();
    window = glfwCreateWindow(params.windowWidth, params.windowHeight, title.c_str(), nullptr, nullptr);
	res = (window != nullptr);

    if (res) {
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwMakeContextCurrent(window);
    }   
    
    return res;
}