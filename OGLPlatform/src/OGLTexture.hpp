//
//  OGLTexture.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include "OGLImageFactory.hpp"

class OGLTexture
{
public:
    OGLTexture(IMAGE_TYPE thirdPartyType);
    ~OGLTexture();
    
    bool LoadTexture(std::string imgFilename);
    void ChangeParameter(GLenum parameter, GLint value) const;
    void ChangeParameter(GLenum parameter, GLfloat value) const;
    void Bind(GLenum textureUnit) const;
    
private:
    GLenum target;
    GLuint textureID;
    IMAGE_TYPE loadingMethod;
};
/* OGLTexture_hpp */
