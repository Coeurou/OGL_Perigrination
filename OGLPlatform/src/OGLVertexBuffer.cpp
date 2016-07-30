//
//  OGLVertexBuffer.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 05/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLVertexBuffer.hpp"

OGLVertexBuffer::OGLVertexBuffer(GLenum target) : target(target)
{
    glGenBuffers(1, &bufferID);
}

OGLVertexBuffer::~OGLVertexBuffer()
{
    glDeleteBuffers(1, &bufferID);
    bufferID = 0;
}

void OGLVertexBuffer::Bind()
{
    glBindBuffer(target, bufferID);
}