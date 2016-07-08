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
    void Bind();
    
private:
    GLuint arrayID;
};
/* OGLVertexArray_hpp */
