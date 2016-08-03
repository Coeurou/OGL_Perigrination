//
//  OGLVertexArray.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "OGLVertexArray.hpp"

OGLVertexArray::OGLVertexArray()
{
    glGenVertexArrays(1, &arrayID);
}

OGLVertexArray::~OGLVertexArray()
{
    glDeleteVertexArrays(1, &arrayID);
    arrayID = 0;
}

void OGLVertexArray::Bind() const
{
    glBindVertexArray(arrayID);
}