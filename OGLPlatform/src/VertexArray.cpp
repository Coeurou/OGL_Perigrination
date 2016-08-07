//
//  VertexArray.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "VertexArray.hpp"

namespace gs
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &arrayID);
    }
    
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &arrayID);
        arrayID = 0;
    }
    
    void VertexArray::BindVAO() const
    {
        glBindVertexArray(arrayID);
    }
}