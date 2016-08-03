//
//  OGLProgram.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLProgram.hpp"

OGLProgram::OGLProgram()
{
	programID = glCreateProgram();
}

OGLProgram::~OGLProgram()
{
	glDeleteProgram(programID);
    programID = 0;
}

void OGLProgram::Attach(GLuint shader)
{
	shaders.insert(shader);
	glAttachShader(programID, shader);
}

void OGLProgram::Detach(GLuint shader)
{
	shaders.erase(shader);
	glDetachShader(programID, shader);
}

void OGLProgram::Link()
{
	glLinkProgram(programID);
}

void OGLProgram::Use() const
{
	glUseProgram(programID);
}