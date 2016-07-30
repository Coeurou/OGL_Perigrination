//
//  OGLTexture.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLTexture.hpp"

OGLTexture::OGLTexture(GLenum type) : target(type)
{
    glGenTextures(1, &textureID);
}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
}

bool OGLTexture::LoadTexture(std::string imgPath)
{
    return false;
}

void OGLTexture::ChangeParameter(GLenum parameter, GLint value)
{
    glTexParameteri(target, parameter, value);
}

void OGLTexture::ChangeParameter(GLenum parameter, GLfloat value)
{
    glTexParameterf(target, parameter, value);
}

void OGLTexture::Bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(target, textureID);
}