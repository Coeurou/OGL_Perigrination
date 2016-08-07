//
//  OGLObject.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 08/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Vertex.hpp"
#include <vector>

namespace gs
{
class OGLObject
{
public:
    OGLObject() {}
    virtual ~OGLObject() {}
    
    virtual bool InitVertices(glm::vec3 origin) = 0;
    std::vector<Vertex> GetVertices() const { return vertices; }
    
protected:
    std::vector<Vertex> vertices;
};
}
/* OGLObject_hpp */
