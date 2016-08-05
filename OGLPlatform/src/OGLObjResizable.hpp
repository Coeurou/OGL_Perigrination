//
//  OGLObjResizable.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 04/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "OGLObject.hpp"

class OGLObjResizable : public OGLObject
{
public:
    OGLObjResizable() : width(1), height(1), depth(1) {}
    virtual ~OGLObjResizable() {}
    
    virtual bool InitVertices(glm::vec3 origin) { return true; }
    
    const float GetWidth() const { return width; }
    const float GetHeight() const { return height; }
    const float GetDepth() const { return depth; }
    virtual void SetWidth(float newWidth) { width = newWidth; }
    virtual void SetHeight(float newHeight) { height = newHeight; }
    virtual void SetDepth(float newDepth) { depth = newDepth; }
    
protected:
    float width;
    float height;
    float depth;
};
/* OGLObjResizable_hpp */
