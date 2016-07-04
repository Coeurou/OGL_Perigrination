//
//  OGLShader.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLShader.hpp"
#include <fstream>

OGLShader::OGLShader(GLenum shaderType)
{
	shaderID = glCreateShader(shaderType);
}

OGLShader::~OGLShader()
{
	glDeleteShader(shaderID);
}

void OGLShader::SetSource(std::string shaderFilePath)
{
	std::ifstream stream(shaderFilePath);
	std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	auto sourcePtr = source.c_str();
	glShaderSource(shaderID, 1, &sourcePtr, nullptr);
}

bool OGLShader::Compile()
{
	glCompileShader(shaderID);
	return false;
}