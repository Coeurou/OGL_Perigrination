//
//  GLFWContext.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 07/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "GLFWContext.hpp"
#include <GLFW/glfw3.h>

GLFWContext::GLFWContext()
{}

GLFWContext::~GLFWContext()
{
    glfwTerminate();
}

bool GLFWContext::InitContext()
{
    bool res = (glfwInit() == GLFW_TRUE);
    
    if (res) {
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
        glfwWindowHint(GLFW_SAMPLES, params.msaaSamples);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.minorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    return res;
}