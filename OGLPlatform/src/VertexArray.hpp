//
//  VertexArray.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "OGLResource.hpp"

namespace gs
{
    class VertexArray : public OGLResource
    {
    public:
        VertexArray();
        ~VertexArray();
        
        const GLuint get() const { return arrayID; }
        void BindVAO() const;
        
    private:
        GLuint arrayID;
    };
}
/* VertexArray_hpp */
