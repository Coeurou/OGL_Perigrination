//
//  OGLVertexArray.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>

class OGLVertexArray
{
public:
    OGLVertexArray();
    ~OGLVertexArray();

	const GLuint get() const { return arrayID; }
    void Bind() const;
    
private:
    GLuint arrayID;
};
/* OGLVertexArray_hpp */
