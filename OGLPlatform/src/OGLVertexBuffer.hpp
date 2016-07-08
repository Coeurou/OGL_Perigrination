//
//  OGLVertexBuffer.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 05/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>

class OGLVertexBuffer
{
public:
    OGLVertexBuffer(GLenum target);
    ~OGLVertexBuffer();
    void Bind();
    
private:
    GLenum target;
    GLuint bufferID;
};
/* OGLVertexBuffer_hpp */
