//
//  OpenGLInfos.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 07/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OpenGLInfos.hpp"
#include <GL/glew.h>
#include <sstream>

OpenGLInfos::OpenGLInfos()
{}

OpenGLInfos::~OpenGLInfos()
{}

std::string OpenGLInfos::GetInfos()
{
    std::stringstream stream;
    stream << "\tOpenGL configuration: " << "\n"<<
    "\tVendor: " << glGetString (GL_VENDOR) << "\n" <<
    "\tRenderer: "<< glGetString (GL_RENDERER) << "\n" <<
    "\tVersion: "<< glGetString (GL_VERSION) << "\n" <<
    "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    return stream.str();
}