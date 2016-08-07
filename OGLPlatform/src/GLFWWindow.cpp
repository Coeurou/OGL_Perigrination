//
//  GLFWWindow.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "GLFWWindow.hpp"
#include <GLFW/glfw3.h>
#include "GLFWContext.hpp"
#include <iostream>
#include "GLFWUtility.hpp"

GLFWWindow::GLFWWindow()
{
    context = std::make_shared<GLFWContext>(nullptr);
}

GLFWWindow::GLFWWindow(GLFWContext ctxt)
{
    context = std::make_shared<GLFWContext>(ctxt);
}

GLFWWindow::~GLFWWindow()
{}

/*void GLFWWindow::RunMainLoop(std::function<void()> oglInstructions)
{
    while (!glfwWindowShouldClose(window->get())) {
        oglInstructions();
        
        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
}*/

bool GLFWWindow::CreateWindow()
{
    bool res = false;
    auto params = context->GetContextParams();
    window = std::unique_ptr<GLFWwindow>(glfwCreateWindow(params.windowWidth, params.windowHeight, title.c_str(), nullptr, nullptr));
    if (res) {
        
        glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwMakeContextCurrent(window.get());
    }
    
    glfwSetErrorCallback(OnGLFWError);
    glfwSetKeyCallback(window.get(), OnKeyDown);
    glfwSetCharCallback(window.get(), OnCharPressed);
    glfwSetScrollCallback(window.get(), OnMouseWheel);
    glfwSetCursorPosCallback(window.get(), OnMouseMove);
    glfwSetWindowSizeCallback(window.get(), OnWindowResize);
    glfwSetMouseButtonCallback(window.get(), OnMouseButtonClick);
    return res;
}