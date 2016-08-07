//
//  VertexBuffer.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 05/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "OGLResource.hpp"

namespace gs
{
    class VertexBuffer : public OGLResource
    {
    public:
        VertexBuffer(GLenum target);
        ~VertexBuffer();
        
        const GLuint get() const { return bufferID; }
        const GLuint GetTarget() const { return target; }
        void BindVBO();
        
    private:
        GLenum target;
        GLuint bufferID;
    };
}
/* VertexBuffer_hpp */
