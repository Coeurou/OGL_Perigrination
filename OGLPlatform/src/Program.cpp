//
//  Program.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Program.hpp"

namespace gs
{
    Program::Program()
    {
        programID = glCreateProgram();
    }
    
    Program::~Program()
    {
        glDeleteProgram(programID);
        programID = 0;
    }
    
    void Program::Attach(GLuint shader)
    {
        shaders.insert(shader);
        glAttachShader(programID, shader);
    }
    
    void Program::Detach(GLuint shader)
    {
        shaders.erase(shader);
        glDetachShader(programID, shader);
    }
    
    void Program::Link()
    {
        glLinkProgram(programID);
    }
    
    void Program::Use() const
    {
        glUseProgram(programID);
    }
}