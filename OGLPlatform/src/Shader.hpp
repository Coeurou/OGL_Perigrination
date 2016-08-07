//
//  Shader.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 28/06/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include "OGLResource.hpp"

namespace gs
{
    class Shader : public OGLResource
    {
    public:
        Shader(GLenum shaderType);
        ~Shader();
        
        const GLuint get() const { return shaderID; }
        bool SetSource(const std::string& shaderFilename);
        bool Compile();
        
        bool operator<(const Shader& rhs) const
        {
            return shaderID < rhs.shaderID;
        }
        
    private:
        GLuint shaderID;
    };
}
/* Shader_hpp */
