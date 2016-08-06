//
//  OGLSampler.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "OGLResource.hpp"

class OGLSampler : public OGLResource
{
    
public:
    OGLSampler();
    ~OGLSampler();
    
    void ChangeParameter(GLenum parameter, GLint value) const;
    void ChangeParameter(GLenum parameter, GLfloat value) const;
    void Bind(int textureUnit) const;
    
    GLuint get() const { return samplerID; }
    GLenum getTextureUnit() const { return texUnit; }
    
private:
    GLuint samplerID;
    GLenum texUnit;
};
/* OGLSampler_hpp */
