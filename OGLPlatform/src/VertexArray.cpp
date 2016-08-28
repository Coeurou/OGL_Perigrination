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
		for (GLuint index : attributes) {
			glDisableVertexAttribArray(index);
		}
        glDeleteVertexArrays(1, &arrayID);
        arrayID = 0;
    }
    
    void VertexArray::BindVAO() const
    {
        glBindVertexArray(arrayID);
    }

	void VertexArray::AddAttribute(GLuint attribLocation, int size, GLenum type, bool normalized, int stride, const void* offset)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, type, normalized, stride, offset);
		attributes.push_back(attribLocation);
	}
}