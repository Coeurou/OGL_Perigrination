//
//  OGLglfw.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLglfw.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLFWUtility.hpp"

OGLglfw::OGLglfw() : window(new OGLWindow), params()
{}

OGLglfw::~OGLglfw()
{
    window = nullptr;
    glfwTerminate();
}

bool OGLglfw::InitGLFW()
{
    bool res = (glfwInit() == GLFW_TRUE) && InitWindow();
    if (res) {
        InitGLFWCallback();
    }
    else {
        std::cerr << "Couldn't initialize the Application." << std::endl;
    }
    
    return res;
}

void OGLglfw::RunMainLoop(std::function<void()> oglInstructions)
{
    while (!glfwWindowShouldClose(window->get())) {
        oglInstructions();
        
        glfwSwapBuffers(window->get());
        glfwPollEvents();
    }
}

bool OGLglfw::InitWindow()
{
    bool res = false;
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    params.minorVersion = 1;
#endif
    glfwWindowHint(GLFW_SAMPLES, params.msaaSamples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    res = window->CreateWindow(params.windowWidth, params.windowHeight, params.title);
    if (res) {
        
        glfwSetInputMode(window->get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwMakeContextCurrent(window->get());
    }
    return res;
}

void OGLglfw::InitGLFWCallback()
{
    glfwSetErrorCallback(OnGLFWError);
    glfwSetKeyCallback(window->get(), OnKeyDown);
    glfwSetCharCallback(window->get(), OnCharPressed);
    glfwSetScrollCallback(window->get(), OnMouseWheel);
    glfwSetCursorPosCallback(window->get(), OnMouseMove);
    glfwSetWindowSizeCallback(window->get(), OnWindowResize);
    glfwSetMouseButtonCallback(window->get(), OnMouseButtonClick);
}