//
//  Sampler.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Sampler.hpp"

namespace gs
{
    Sampler::Sampler()
    {
        glGenSamplers(1, &samplerID);
    }
    
    Sampler::~Sampler()
    {
        glDeleteSamplers(1, &samplerID);
        samplerID = 0;
    }
    
    void Sampler::ChangeParameter(GLenum parameter, GLint value) const
    {
        glSamplerParameteri(samplerID, parameter, value);
    }
    
    void Sampler::ChangeParameter(GLenum parameter, GLfloat value) const
    {
        glSamplerParameterf(samplerID, parameter, value);
    }
    
    void Sampler::BindSampler(int textureUnit) const
    {
        glBindSampler(textureUnit, samplerID);
    }
}