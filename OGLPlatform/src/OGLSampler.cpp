//
//  OGLSampler.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLSampler.hpp"

OGLSampler::OGLSampler()
{
    glGenSamplers(1, &samplerID);
}

OGLSampler::~OGLSampler()
{
    glDeleteSamplers(1, &samplerID);
    samplerID = 0;
}

void OGLSampler::ChangeParameter(GLenum parameter, GLint value) const
{
    glSamplerParameteri(samplerID, parameter, value);
}

void OGLSampler::ChangeParameter(GLenum parameter, GLfloat value) const
{
    glSamplerParameterf(samplerID, parameter, value);
}

void OGLSampler::Bind(int textureUnit) const
{
    glBindSampler(textureUnit, samplerID);
}