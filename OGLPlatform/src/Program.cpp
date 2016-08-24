//
//  Program.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Program.hpp"
#include <vector>
#include <iostream>

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
    
    bool Program::Link()
    {
		GLint res = GL_FALSE;

		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &res);
		if (res == GL_FALSE) {
			GLint logLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> log(logLength);
			glGetProgramInfoLog(programID, logLength, &logLength, &log[0]);
			for (auto c : log)
			{
				std::cerr << c;
			}
			std::cerr << std::endl;
		}

		return (res == GL_TRUE);
    }
    
    void Program::Use() const
    {
        glUseProgram(programID);
    }
}