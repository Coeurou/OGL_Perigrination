//
//  OGLglfw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Window.hpp"
#include <memory>
#include <functional>

class GLFWContext;
class GLFWwindow;

class GLFWWindow : public gs::Window
{
public:
    GLFWWindow();
    GLFWWindow(GLFWContext ctxt);
    ~GLFWWindow();
    
    GLFWwindow* get() const { return window.get(); }
    bool CreateWindow();
    
private:
    std::unique_ptr<GLFWwindow> window;
};