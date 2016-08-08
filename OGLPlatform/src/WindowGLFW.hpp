//
//  OGLglfw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>

class GLFWContext;

class WindowGLFW : public gs::Window
{
public:
    WindowGLFW();
    WindowGLFW(GLFWContext* ctxt);
    ~WindowGLFW();
    
    GLFWwindow* get() const { return window; }
    bool CreateWindow();
    
private:
    GLFWwindow* window;
};