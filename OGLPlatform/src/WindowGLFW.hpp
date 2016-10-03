//
//  OGLglfw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Window.hpp"
#include <GLFW/glfw3.h>

class GLFWContext;

class WindowGLFW : public gs::Window
{
public:
    WindowGLFW();
    WindowGLFW(gs::Context* ctxt);
    ~WindowGLFW();
    
    GLFWwindow* get() const { return window; }
    bool CreateWindow() override;
	bool Render() override;
    
private:
    GLFWwindow* window;

	void InitCallbacks();
};