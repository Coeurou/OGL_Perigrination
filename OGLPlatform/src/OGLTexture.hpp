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

class OGLTexture
{
public:
    OGLTexture(GLenum type);
    ~OGLTexture();
    
    bool LoadTexture(std::string imgPath);
    void ChangeParameter(GLenum parameter, GLint value);
    void ChangeParameter(GLenum parameter, GLfloat value);
    void Bind(GLenum textureUnit);
    
private:
    GLenum target;
    GLuint textureID;
};

/* OGLTexture_hpp */
