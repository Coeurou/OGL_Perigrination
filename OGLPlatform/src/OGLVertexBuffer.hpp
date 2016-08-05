//
//  OGLVertexBuffer.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 05/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "OGLResource.hpp"

class OGLVertexBuffer : public OGLResource
{
public:
    OGLVertexBuffer(GLenum target);
    ~OGLVertexBuffer();

    const GLuint get() const { return bufferID; }
    const GLuint getTarget() const { return target; }
    void Bind();
    
private:
    GLenum target;
    GLuint bufferID;
};
/* OGLVertexBuffer_hpp */
