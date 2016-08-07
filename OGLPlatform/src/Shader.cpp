//
//  Shader.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Shader.hpp"
#include "OGLApplicationConstants.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace gs
{
    Shader::Shader(GLenum shaderType)
    {
        shaderID = glCreateShader(shaderType);
    }
    
    Shader::~Shader()
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
    
    bool Shader::SetSource(const std::string& shaderFilename)
    {
        std::ifstream stream(ShadersPath + "/" + shaderFilename);
        bool res = stream.is_open();
        if (res) {
            std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
            auto sourcePtr = source.c_str();
            glShaderSource(shaderID, 1, &sourcePtr, nullptr);
        }
        else {
            std::cerr << "Shader file not found: " << shaderFilename << std::endl;
        }
        return res;
    }
    
    bool Shader::Compile()
    {
        GLint res = GL_FALSE;
        
        glCompileShader(shaderID);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &res);
        if (res == GL_FALSE) {
            GLint logLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(shaderID, logLength, &logLength, &log[0]);
            for (auto c : log)
            {
                std::cerr << c;
            }
            std::cerr << std::endl;
        }
        
        return (res == GL_TRUE);
    }
}