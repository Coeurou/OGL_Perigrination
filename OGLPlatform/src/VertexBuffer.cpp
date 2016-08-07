//
//  VertexBuffer.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 05/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "VertexBuffer.hpp"

namespace gs
{
    VertexBuffer::VertexBuffer(GLenum target) : target(target)
    {
        glGenBuffers(1, &bufferID);
    }
    
    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &bufferID);
        bufferID = 0;
    }
    
    void VertexBuffer::BindVBO()
    {
        glBindBuffer(target, bufferID);
    }
}