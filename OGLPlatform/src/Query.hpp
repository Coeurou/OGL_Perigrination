//
//  Query.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 13/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "OGLResource.hpp"

namespace gs
{
    class Query : public OGLResource
    {
        
    public:
        Query(GLenum target);
        Query(const Query& q);
        ~Query();
        
        void BeginQuery() const;
        void EndQuery() const;
        
        GLuint get() const { return queryID; }
        GLenum GetTarget() const { return target; }
        
    private:
        GLuint queryID;
        GLenum target;
    };
}


/* Query_hpp */
