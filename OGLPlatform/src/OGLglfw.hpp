//
//  OGLglfw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 23/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include "OGLWindow.hpp"
#include <memory>
#include <functional>

class OGLglfw : private Uncopyable
{
public:
    OGLglfw();
    ~OGLglfw();
    
    bool InitGLFW();
    void RunMainLoop(std::function<void()> oglInstructions);
    
    const OGLWindow& GetGLFWWindow() const { return *window; }
    void SetMinorVersion(int version) { params.minorVersion = version; }
    void SetMajorVersion(int version) { params.majorVersion = version; }
    void SetSamplesNumber(int samples) { params.msaaSamples = samples; }
    void SetWindowTitle(const std::string& title) { params.title = title; }
    
private:
    bool InitWindow();
    void InitGLFWCallback();
    
    std::unique_ptr<OGLWindow> window;
    struct GLFWParams
    {
        int minorVersion;
        int majorVersion;
        int msaaSamples;
        int windowHeight;
        int windowWidth;
        std::string title;
        GLFWParams() : minorVersion(5), majorVersion(4), msaaSamples(4),
                       windowWidth(1024), windowHeight(720), title("") {}
    } params;
};