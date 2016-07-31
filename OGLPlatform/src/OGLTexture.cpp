//
//  OGLTexture.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLTexture.hpp"
#include "OGLApplicationConstants.hpp"

OGLTexture::OGLTexture(IMAGE_TYPE thirdPartyType) : loadingMethod(thirdPartyType)
{}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
}

bool OGLTexture::LoadTexture(std::string imgFilename)
{
    imgFilename = TexturesPath + "/" + imgFilename;
    
    auto texImg = OGLImageFactory::CreateImage(loadingMethod);
    textureID = texImg->LoadImageFile(imgFilename);
    
    bool res = (textureID != 0);
    if (res) {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    return res;
}

void OGLTexture::ChangeParameter(GLenum parameter, GLint value) const
{
    glTexParameteri(target, parameter, value);
}

void OGLTexture::ChangeParameter(GLenum parameter, GLfloat value) const
{
    glTexParameterf(target, parameter, value);
}

void OGLTexture::Bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(target, textureID);
}