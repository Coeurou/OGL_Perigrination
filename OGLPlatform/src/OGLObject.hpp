//
//  OGLObject.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "OGLVertex.hpp"
#include <vector>

class OGLObject
{
public:
    OGLObject() {}
    virtual ~OGLObject() {}
    
    virtual bool InitVertices(glm::vec3 origin) = 0;
    std::vector<OGLVertex> GetVertices() const { return vertices; }
    
protected:
    std::vector<OGLVertex> vertices;
};
/* OGLObject_hpp */
